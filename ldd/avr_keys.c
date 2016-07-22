#include <linux/module.h>
#include <linux/errno.h>
#include <linux/input.h>
#include <linux/i2c.h>
#include <linux/slab.h>
#include <linux/workqueue.h>

#define I2C_ID 0x10

struct avr_kpad {
	struct i2c_client *client;
	struct input_dev *input;
	struct delayed_work work;
	unsigned long delay;
};

static int avr_kpad_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	struct avr_kpad *kpad;
	struct input_dev *input;
	int error;

	kpad = kzalloc(sizeof(struct avr_kpad), GFP_KERNEL);
	input = input_allocate_device();
	if( !kpad || !input ) {
		error = -ENOMEM;
		goto free_mem;
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
		goto free_mem;
	}

	device_init_wakeup(&client->dev, 1);
	i2c_set_clientdata(client, kpad);
	
	dev_info(&client->dev, "Rev.%d keypad\n", 0x01);
	return 0;

	

//unreg_dev:
	//input_unregister_device(input);
free_mem:
	input_free_device(input);
	kfree(kpad);
	return error;
}

static int avr_kpad_remove(struct i2c_client *client)
{
	struct avr_kpad *kpad = i2c_get_clientdata(client);

	input_unregister_device(kpad->input);
	kfree(kpad);

	return 0;
}

static const struct i2c_device_id avr_id[] = {
	{ "avr-keys", 0 },
	{ }
};

MODULE_DEVICE_TABLE(i2c, avr_id);

static struct i2c_driver avr_driver = {
	.driver = {
		.name = KBUILD_MODNAME,
	},
	.probe	= avr_kpad_probe,
	.remove = avr_kpad_remove,
	.id_table = avr_id,
};

static struct i2c_board_info avr_kpad_board_info[] __initdata = {
	{
		I2C_BOARD_INFO("avr-keys", I2C_ID),
	},
};

static int __init avr_kpad_init(void)
{
	i2c_register_board_info(1, avr_kpad_board_info, ARRAY_SIZE(avr_kpad_board_info));
	i2c_add_driver( &avr_driver );
	return 0;
}

static void __exit avr_kpad_exit(void)
{
}

module_init( avr_kpad_init );
module_exit( avr_kpad_exit );

MODULE_LICENSE("GPL");
MODULE_AUTHOR("KrisMT <>");
MODULE_DESCRIPTION("AVR I2C Keypad driver");

