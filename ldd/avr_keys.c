#include <linux/module.h>
#include <linux/errno.h>
#include <linux/input.h>
#include <linux/i2c.h>
#include <linux/slab.h>
#include <linux/workqueue.h>

#define I2C_ID 0x0F

const u8 lkc[] = {
  //  0     1     2     3     4     5     6     7     8     9     a     b     c     d     e     f
  0x00, 0x00, 0x00, 0x00, 0x1E, 0x30, 0x2E, 0x20, 0x12, 0x21, 0x22, 0x23, 0x17, 0x24, 0x25, 0x26, //0x00
  0x32, 0x31, 0x18, 0x19, 0x10, 0x13, 0x1F, 0x14, 0x16, 0x2F, 0x11, 0x2D, 0x15, 0x2C, 0x02, 0x03, //0x10
  0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x1C, 0x01, 0x0E, 0x0F, 0x39, 0x0C, 0x0D, 0x1A, //0x20
  0x1B, 0x2B, 0x2B, 0x27, 0x28, 0x29, 0x33, 0x34, 0x35, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, 0x40, //0x30
  0x41, 0x42, 0x43, 0x44, 0x57, 0x58, 0x63, 0x46, 0x77, 0x6E, 0x66, 0x68, 0x6F, 0x6B, 0x6D, 0x6A, //0x40
  0x69, 0x6C, 0x67, 0x45, 0x62, 0x37, 0x4A, 0x4E, 0x60, 0x4F, 0x50, 0x51, 0x4B, 0x4C, 0x4D, 0x47, //0x50
  0x48, 0x49, 0x52, 0x53, 0x56, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //0x60
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x71, //0x70
  0x73, 0x72, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //0x80
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //0x90
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //0xA0
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //0xB0 **
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //0xC0 **
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //0xD0
  0x1D, 0x2A, 0x38, 0x7D, 0x61, 0x36, 0x64, 0x7E, 0xA4, 0xA6, 0xA5, 0xA3, 0xA1, 0x73, 0x72, 0x71, //0xE0 **
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  //0xF0
};

typedef struct {
  unsigned char modified;
  unsigned char report_id;
  unsigned char keys[13];
  unsigned char key;
}report_t;

struct avr_kpad {
  struct i2c_client *client;
  struct input_dev *input;
  struct delayed_work work;
  unsigned long delay;

  //Key event buffer
  unsigned char buff[16];
  unsigned char old_key[256];
  unsigned char new_key[256];
};

//struct timer_list time;

struct avr_kpad *gkpad;

/*
void avr_kpad_report(unsigned long para)
{
  i2c_client* client = gkpad->client;
  unsigned char addr, i, j;
  report_t *report;
  unsigned char *tmp_ptr;
  unsigned char mod;
  unsigned int sum = 0;

  memset(gkpad->new_key, 0, 256);


  //Get status key from I2C BUS
  for( addr=0; addr<16; addr++) {
    gkpad->buff[addr] = i2c_smbus_read_data(client, addr);
    sum += gkpad->buff[addr];
  }

  // Take new key report event, when 'NEW' key is down
  report = (report_t*)(gkpad->buff);
  tmp_ptr = gkpad->new_key;
  mod = report->modified;

  for( addr=0; addr<8; addr++) {
    tmp_ptr[0xE0 + addr] = (mod & 0x01);
    mod = mod >> 1;
  }

  for( i=0; i<13; i++) {
    mod = report->keys[i];
    for( j=0; j<8; j++) {
      tmp_ptr[i*8 + j] = (mod & 0x01);
      mod = mod >> 1;
    }
  }

  if( report->key ) {
    tmp_ptr[report->key] = 1;
  }

  // Report key event when 'NEW' key is UP
  if( !sum ) {
    for( i=1; i<256; i++) {
      mod = (tmp_ptr[i])?lkc[i]:0;

      input_report_key(input, mod, 1);
      input_sync(input);
      input_report_key(input, mod, 0);
      input_sync(input);
    }
  }

  // swap key buffer
  tmp_ptr = old_key;
  old_key = new_key;
  new_key = tmp_ptr;

  mod_timer(&timer, jiffies);
}
*/

//Delayed work or timer


/*************************
 *
 * I2C Proved function
 *
 *************************/
static int avr_kpad_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
  struct avr_kpad *kpad;
  struct input_dev *input;
  int error;
  int i;

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
  input->id.vendor = 0xFADE;
  input->id.product = 0x0001;
  input->id.version = 0x01;

  /**** Input device Mask bit ****/
  input->evbit[0] = BIT_MASK(EV_KEY);
  for (i=0; i<256; i++)
    set_bit(i, input->keybit);
  clear_bit(0, input->keybit);

  /**** Register input device ****/
  error = input_register_device(input);
  if( error ) {
    dev_err(&client->dev, "unable to register input device\n");
    goto unregis_input;
  }

  device_init_wakeup(&client->dev, 1);
  i2c_set_clientdata(client, kpad);

  dev_info(&client->dev, "Proved device 0x%02X on I2C BUS\n", client->addr);

  /*
     init_timer(&timer);
     timer.expires = jiffies + HZ;
     timer.function = avr_kpad_report;
     add_timer(&timer);
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

/*************************
 *
 * I2C Removed function
 *
 *************************/
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

/*************************
 *
 * I2C Init function
 *
 *************************/
static int __init avr_kpad_init(void)
{
  struct i2c_client *client;
  struct i2c_adapter *adapter;
  int ret;

  adapter = i2c_get_adapter(1);
  dev_info(&adapter->dev, "Get I2C adapter\n");

  client = i2c_new_device(adapter, avr_kpad_board_info);
  dev_info(&client->dev,"Detected client addr 0x%02X\n", client->addr);

  ret = i2c_add_driver( &avr_driver );
  if( ret < 0) {
    dev_err(&client->dev, "Init cann't add driver to I2C device 0x%02X\n", client->addr);

  }
  return 0;
}

/*************************
 *
 * I2C Exit function
 *
 *************************/
static void __exit avr_kpad_exit(void)
{
  struct i2c_client *client = gkpad->client;
  i2c_unregister_device( client );
  i2c_del_driver( &avr_driver );
}

module_init( avr_kpad_init );
module_exit( avr_kpad_exit );

MODULE_LICENSE("GPL");
MODULE_AUTHOR("KrisMT <ratiodetector{[00at00]}hotmail.com>");
MODULE_DESCRIPTION("AVR I2C Keypad driver");
