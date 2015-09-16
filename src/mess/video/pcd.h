// license:BSD-3-Clause
// copyright-holders:Carl

#ifndef _PCD_H_
#define _PCD_H_

#include "emu.h"
#include "machine/pic8259.h"
#include "video/scn2674.h"

#define MCFG_PCX_VIDEO_TXD_HANDLER(_devcb) \
	devcb = &pcx_video_device::set_txd_handler(*device, DEVCB_##_devcb);

class pcdx_video_device : public device_t
{
public:
	pcdx_video_device(const machine_config &mconfig, device_type type, const char *name, const char *tag, device_t *owner, UINT32 clock, const char *shortname, const char *source);

	virtual DECLARE_ADDRESS_MAP(map, 16) = 0;
	DECLARE_READ8_MEMBER(detect_r);
	DECLARE_WRITE8_MEMBER(detect_w);
protected:
	required_device<cpu_device> m_maincpu;
	required_device<cpu_device> m_mcu;
	required_device<scn2674_device> m_crtc;
	required_device<palette_device> m_palette;
	required_device<gfxdecode_device> m_gfxdecode;
};

class pcd_video_device : public pcdx_video_device
{
public:
	pcd_video_device(const machine_config &mconfig, const char *tag, device_t *owner, UINT32 clock);

	DECLARE_ADDRESS_MAP(map, 16);
	DECLARE_ADDRESS_MAP(vram_map, 8);
	DECLARE_READ8_MEMBER(mcu_r);
	DECLARE_WRITE8_MEMBER(mcu_w);
	DECLARE_WRITE8_MEMBER(vram_sw_w);
	DECLARE_READ8_MEMBER(vram_r);
	DECLARE_WRITE8_MEMBER(vram_w);

	virtual const rom_entry *device_rom_region() const;
	virtual machine_config_constructor device_mconfig_additions() const;
	SCN2674_DRAW_CHARACTER_MEMBER(display_pixels);
protected:
	void device_start();
	void device_reset();
private:
	dynamic_buffer m_vram;
	dynamic_buffer m_charram;
	UINT8 m_vram_sw;
};

class pcx_video_device : public pcdx_video_device,
						 public device_serial_interface
{
public:
	pcx_video_device(const machine_config &mconfig, const char *tag, device_t *owner, UINT32 clock);
	template<class _Object> static devcb_base &set_txd_handler(device_t &device, _Object object) { return downcast<pcx_video_device &>(device).m_txd_handler.set_callback(object); }

	DECLARE_ADDRESS_MAP(map, 16);
	DECLARE_READ8_MEMBER(term_r);
	DECLARE_WRITE8_MEMBER(term_w);
	DECLARE_READ8_MEMBER(term_mcu_r);
	DECLARE_WRITE8_MEMBER(term_mcu_w);
	DECLARE_READ8_MEMBER(rx_callback);
	DECLARE_WRITE8_MEMBER(tx_callback);
	DECLARE_READ8_MEMBER(vram_r);
	DECLARE_WRITE8_MEMBER(vram_w);
	DECLARE_READ8_MEMBER(vram_latch_r);
	DECLARE_WRITE8_MEMBER(vram_latch_w);
	DECLARE_READ8_MEMBER(unk_r);
	DECLARE_WRITE8_MEMBER(p1_w);

	virtual const rom_entry *device_rom_region() const;
	virtual machine_config_constructor device_mconfig_additions() const;
	SCN2674_DRAW_CHARACTER_MEMBER(display_pixels);
protected:
	void device_start();
	void device_reset();
	void tra_callback();
	void rcv_complete();
	void device_timer(emu_timer &timer, device_timer_id id, int param, void *ptr);
private:
	dynamic_buffer m_vram;
	required_region_ptr<UINT8> m_charrom;
	required_device<pic8259_device> m_pic2;
	devcb_write_line m_txd_handler;
	UINT8 m_term_key, m_term_char, m_term_stat, m_vram_latch_r[2], m_vram_latch_w[2], m_p1;
};

extern const device_type PCD_VIDEO;
extern const device_type PCX_VIDEO;

#endif