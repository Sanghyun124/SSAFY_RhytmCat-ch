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
	{ 0x3c4bd21, "i2c_del_driver" },
	{ 0x4a2c6405, "i2c_unregister_device" },
	{ 0x1e2149ae, "i2c_register_driver" },
	{ 0xc2842662, "i2c_put_adapter" },
	{ 0x16ceb625, "i2c_new_client_device" },
	{ 0x884b8f93, "i2c_get_adapter" },
	{ 0x8f678b07, "__stack_chk_guard" },
	{ 0x3ea1b6e4, "__stack_chk_fail" },
	{ 0x51a910c0, "arm_copy_to_user" },
	{ 0x7c580234, "i2c_smbus_read_i2c_block_data" },
	{ 0x7631fa47, "i2c_smbus_write_byte_data" },
	{ 0x3a8001ae, "__register_chrdev" },
	{ 0x6bc3fbc0, "__unregister_chrdev" },
	{ 0xb1ad28e0, "__gnu_mcount_nc" },
};

MODULE_INFO(depends, "");

MODULE_ALIAS("i2c:mpu6050");

MODULE_INFO(srcversion, "4EEE03029D2B61BEDFFDCE0");
