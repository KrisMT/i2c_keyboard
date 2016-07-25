#include <linux/module.h>
#include <linux/errno.h>
#include <linux/input.h>
#include <linux/i2c.h>
#include <linux/slab.h>
#include <linux/workqueue.h>

#define I2C_ID 0x0F

struct avr_kpad {
	struct i2c_client *client;
	struct input_dev *input;
	struct delayed_work work;
	unsigned long delay;

	unsigned char buff[256];
};

struct avr_kpad *gkpad;

static int avr_kpad_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	struct avr_kpad *kpad;
	struct input_dev *input;
	int error;

	if (!i2c_check_functionality(client->adapter, I2C_FUNC_SMBUS_BYTE_DATA)) {
		dev_err(&client->dev, "SMBUS Byte Data not Supported\n");
		return -EIO;
	}

	kpad = kzalloc(sizeof(struct avr_kpad), GFP_KERNEL);

	if( !kpad ) {
		dev_err(&client->dev, "Cann't allocate kpad\n");
		error = -ENOMEM;
		goto free_kpad;
	}

	gkpad = kpad;

	input = input_allocate_device();
	if( !input ) {
		dev_err(&client->dev, "Cann't allocate input device\n");
		error = -ENOMEM;
		goto free_input;
	}

	kpad->client = client;
	kpad->input = input;

	input->name = client->name;
	input->phys = "avr-i2c-keys/input0";
	input->dev.parent = &client->dev;

	input_set_drvdata(input, kpad);
	input->id.bustype = BUS_I2C;
	input->id.vendor = 0xDECA;
	input->id.product = 0x0001;
	input->id.version = 0x01;

	error = input_register_device(input);
	if( error ) {
		dev_err(&client->dev, "unable to register input device\n");
		goto unregis_input;
	}

	device_init_wakeup(&client->dev, 1);
	i2c_set_clientdata(client, kpad);
	
	dev_info(&client->dev, "Proved device 0x%02X on I2C BUS\n", client->addr);
/*
	dev_info(&client->dev, "Test read at 0xC0: 0x%02X \n", i2c_smbus_read_byte_data(client, 0xC0) );
	i2c_smbus_write_byte_data(client, 0x00, 0x4b);
	dev_info(&client->dev, "Test read at 0x00: 0x%02X \n", i2c_smbus_read_byte_data(client, 0x00) );
	//i2c_master_recv(client, kpad->buff, 256);
*/
	return 0;

unregis_input:
	input_unregister_device(input);
free_input:
	input_free_device(input);
free_kpad:
	kfree(kpad);
	return error;
}

/*
 * Driver remove
 */
static int avr_kpad_remove(struct i2c_client *client)
{
	struct avr_kpad *kpad = i2c_get_clientdata(client);

	dev_info(&client->dev, "Removed device 0x%02X from I2C BUS\n", client->addr);
	input_unregister_device(kpad->input);
	input_free_device(kpad->input);
	kfree(kpad);

	return 0;
}

static const struct i2c_device_id avr_id[] = {
	{ "avr_keys", 0 },
	{ }
};

MODULE_DEVICE_TABLE(i2c, avr_id);

static struct i2c_driver avr_driver = {
	.driver = {
		.name = "avr_keys",
	},
	.probe	= avr_kpad_probe,
	.remove = avr_kpad_remove,
	.id_table = avr_id,
};

static struct i2c_board_info avr_kpad_board_info[] __initdata = {
	{
	.type = "avr_keys",
	.addr = I2C_ID,
	},
	{ }
};

/*
 * Driver init
 */

static int __init avr_kpad_init(void)
{
	struct i2c_client *client;
	struct i2c_adapter *adapter;
	int ret;

	adapter = i2c_get_adapter(1);
	printk("Detected Adapter 0x%08X\n", (unsigned int)adapter);

	client = i2c_new_device(adapter, avr_kpad_board_info);
	dev_info(&client->dev,"Detected client addr 0x%02X\n", client->addr);

	ret = i2c_add_driver( &avr_driver );
	if( ret < 0) {
		printk("Init cann't add driver to I2C device 0x%02X\n", client->addr);

	}
	return 0;
}

/*
 * Driver exit
 */
static void __exit avr_kpad_exit(void)
{
	struct i2c_client *client = gkpad->client;
	i2c_unregister_device( client );
	i2c_del_driver( &avr_driver );
}

module_init( avr_kpad_init );
module_exit( avr_kpad_exit );

MODULE_LICENSE("GPL");
MODULE_AUTHOR("KrisMT <>");
MODULE_DESCRIPTION("AVR I2C Keypad driver");
