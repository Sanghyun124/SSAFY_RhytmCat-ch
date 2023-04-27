#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xb181834b, "module_layout" },
	{ 0xe5a65a79, "i2c_del_driver" },
	{ 0x2f3cec4c, "i2c_register_driver" },
	{ 0x8f678b07, "__stack_chk_guard" },
	{ 0x3ea1b6e4, "__stack_chk_fail" },
	{ 0x2cfde9a2, "warn_slowpath_fmt" },
	{ 0x51a910c0, "arm_copy_to_user" },
	{ 0xc358aaf8, "snprintf" },
	{ 0xe1a2c145, "i2c_smbus_read_i2c_block_data" },
	{ 0x3d43bc4d, "i2c_smbus_write_byte_data" },
	{ 0x9db60ebb, "device_create" },
	{ 0x9b9521, "__class_create" },
	{ 0x1966a4ce, "__register_chrdev" },
	{ 0x92997ed8, "_printk" },
	{ 0x6bc3fbc0, "__unregister_chrdev" },
	{ 0xd3c08686, "class_destroy" },
	{ 0xc3b51f61, "device_destroy" },
	{ 0xb1ad28e0, "__gnu_mcount_nc" },
};

MODULE_INFO(depends, "");

MODULE_ALIAS("i2c:mpu6050");

MODULE_INFO(srcversion, "3D7A5D841969ABDA50B7F1C");
