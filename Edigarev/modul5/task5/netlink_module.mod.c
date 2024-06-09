#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/export-internal.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

#ifdef CONFIG_UNWINDER_ORC
#include <asm/orc_header.h>
ORC_HEADER;
#endif

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
	{ 0xc3920a5a, "init_net" },
	{ 0x7c9dac2e, "__netlink_kernel_create" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0xa1f25452, "netlink_kernel_release" },
	{ 0xf1f012c6, "__alloc_skb" },
	{ 0x2aa4c83d, "__nlmsg_put" },
	{ 0x2756c0b8, "netlink_unicast" },
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0x122c3a7e, "_printk" },
	{ 0x2fa5cadd, "module_layout" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "95B216565E5A30976B2DE28");