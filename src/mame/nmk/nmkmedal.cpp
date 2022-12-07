// license:BSD-3-Clause
// copyright-holders:

// Skeleton driver for medal games on NMK hardware.

#include "emu.h"

#include "cpu/tlcs90/tlcs90.h"
#include "machine/nmk112.h"
#include "sound/okim6295.h"
#include "sound/okim6376.h"

#include "speaker.h"


/*
Trocana by NMK / NTC. Possibly distributed by Face?

Video of the game: https://www.youtube.com/watch?v=s63Gokcyn8M
Recording of some of the music: https://www.youtube.com/watch?v=TZMr-MX_M0w

PCBs:

NMK MEC95110 - main CPU board
- Toshiba TMP90C041AN
- 16.5000 MHz XTAL
- main CPU ROM
- Oki M6650
- OKI ROM
- 8 x connectors

NMK MEC95110-SUB2
- 4 x connectors

NMK MEC95110-SUB3
- 3 x connectors

-------
Happy Pierrot by NMK

Video of the game: https://www.youtube.com/watch?v=t4-IN1v_DOQ

PCBs:

NMK MAC96112 - main CPU board, stickered 961409
- Toshiba TMP90C041AN
- 16.0000 MHz XTAL
- main CPU ROM
- Oki M6295
- Oki ROM
- 2 x 8-dip banks
- 7 x connectors

Other PCBs unknown

-------
Dream Rail by NMK

PCBs:

NMK MAC94104 - main CPU board, stickered 957G. 0144
- Toshiba TMP90C041? Empty socket marked TMP041
- 16.0000 MHz XTAL
- main CPU ROM
- Oki M6295
- Oki ROM
- NMK005 custom
- NMK112 custom
- 2 x 8-dip banks
- 5 x connectors

Other PCBs unknown
*/


namespace {

class nmkmedal_state : public driver_device
{
public:
	nmkmedal_state(const machine_config &mconfig, device_type type, const char *tag)
		: driver_device(mconfig, type, tag)
		, m_maincpu(*this, "maincpu")
	{ }

protected:
	required_device<cpu_device> m_maincpu;
};

class trocana_state : public nmkmedal_state
{
public:
	trocana_state(const machine_config &mconfig, device_type type, const char *tag)
		: nmkmedal_state(mconfig, type, tag)
		, m_oki(*this, "oki")
	{ }

	void trocana(machine_config &config);

private:
	void adpcm_control_w(u8 data);

	void mem_map(address_map &map);

	required_device<okim6650_device> m_oki;
};

class hpierrot_state : public nmkmedal_state
{
public:
	hpierrot_state(const machine_config &mconfig, device_type type, const char *tag)
		: nmkmedal_state(mconfig, type, tag)
		, m_oki(*this, "oki")
	{ }

	void drail(machine_config &config);
	void hpierrot(machine_config &config);

private:
	void drail_mem_map(address_map &map);
	void mem_map(address_map &map);

	required_device<okim6295_device> m_oki;
};


void trocana_state::adpcm_control_w(u8 data)
{
	m_oki->ch2_w(BIT(data, 0));
	m_oki->cmd_w(BIT(data, 1));
	m_oki->st_w(BIT(data, 2) || !BIT(data, 1));
}

void trocana_state::mem_map(address_map &map)
{
	map(0x0000, 0x7fff).rom().region("maincpu", 0);
	map(0x8000, 0x8000).portr("IN0");
	map(0x8001, 0x8001).portr("IN1");
	map(0x8002, 0x8002).portr("IN2");
	map(0x8003, 0x8003).portr("IN3");
	map(0x8004, 0x8004).portr("IN4");
	map(0x8005, 0x8005).portr("IN5");
	map(0xa003, 0xa003).w(m_oki, FUNC(okim6376_device::write));
	map(0xa004, 0xa004).w(FUNC(trocana_state::adpcm_control_w));
	map(0xc000, 0xc7ff).ram();
}

void hpierrot_state::mem_map(address_map &map)
{
	map(0x0000, 0x7fff).rom().region("maincpu", 0);
	//map(0xa800, 0xa80f).w() // to the mechanical parts?? or leds / lamps?
	map(0xc000, 0xc7ff).ram();
}

void hpierrot_state::drail_mem_map(address_map &map)
{
	map(0x0000, 0x7fff).rom().region("maincpu", 0);
	map(0x8004, 0x8004).portr("IN4"); // probably 1 of the 2 dip banks
	map(0x8005, 0x8005).portr("IN5"); // "
	map(0x8800, 0x8800).rw("oki", FUNC(okim6295_device::read), FUNC(okim6295_device::write));
	map(0x8820, 0x8827).w("nmk112", FUNC(nmk112_device::okibank_w));
	map(0xa000, 0xa000).portr("IN0");
	map(0xa001, 0xa001).portr("IN1");
	map(0xa002, 0xa002).portr("IN2");
	//map(0xa800, 0xa80f).w() // to the mechanical parts?? or leds / lamps?
	map(0xc000, 0xc7ff).ram();
}

static INPUT_PORTS_START( trocana )
	PORT_START("IN0")
	PORT_BIT(0x01, IP_ACTIVE_LOW, IPT_OTHER) PORT_NAME("0.0") PORT_CODE(KEYCODE_1)
	PORT_BIT(0x02, IP_ACTIVE_LOW, IPT_OTHER) PORT_NAME("0.1") PORT_CODE(KEYCODE_2)
	PORT_BIT(0x04, IP_ACTIVE_LOW, IPT_OTHER) PORT_NAME("0.2") PORT_CODE(KEYCODE_3)
	PORT_BIT(0x08, IP_ACTIVE_LOW, IPT_OTHER) PORT_NAME("0.3") PORT_CODE(KEYCODE_4)
	PORT_BIT(0x10, IP_ACTIVE_LOW, IPT_OTHER) PORT_NAME("0.4") PORT_CODE(KEYCODE_5)
	PORT_BIT(0x20, IP_ACTIVE_LOW, IPT_OTHER) PORT_NAME("0.5") PORT_CODE(KEYCODE_6)
	PORT_BIT(0x40, IP_ACTIVE_LOW, IPT_OTHER) PORT_NAME("0.6") PORT_CODE(KEYCODE_7)
	PORT_BIT(0x80, IP_ACTIVE_LOW, IPT_OTHER) PORT_NAME("0.7") PORT_CODE(KEYCODE_8)

	PORT_START("IN1")
	PORT_BIT(0x01, IP_ACTIVE_LOW, IPT_OTHER) PORT_NAME("1.0") PORT_CODE(KEYCODE_Q)
	PORT_BIT(0x02, IP_ACTIVE_LOW, IPT_OTHER) PORT_NAME("1.1") PORT_CODE(KEYCODE_W)
	PORT_BIT(0x04, IP_ACTIVE_LOW, IPT_OTHER) PORT_NAME("1.2") PORT_CODE(KEYCODE_E)
	PORT_BIT(0x08, IP_ACTIVE_LOW, IPT_OTHER) PORT_NAME("1.3") PORT_CODE(KEYCODE_R)
	PORT_BIT(0x10, IP_ACTIVE_LOW, IPT_OTHER) PORT_NAME("1.4") PORT_CODE(KEYCODE_T)
	PORT_BIT(0x20, IP_ACTIVE_LOW, IPT_OTHER) PORT_NAME("1.5") PORT_CODE(KEYCODE_Y)
	PORT_BIT(0x40, IP_ACTIVE_LOW, IPT_OTHER) PORT_NAME("1.6") PORT_CODE(KEYCODE_U)
	PORT_BIT(0x80, IP_ACTIVE_LOW, IPT_OTHER) PORT_NAME("1.7") PORT_CODE(KEYCODE_I)

	PORT_START("IN2")
	PORT_BIT(0x01, IP_ACTIVE_LOW, IPT_OTHER) PORT_NAME("2.0") PORT_CODE(KEYCODE_A)
	PORT_BIT(0x02, IP_ACTIVE_LOW, IPT_OTHER) PORT_NAME("2.1") PORT_CODE(KEYCODE_S)
	PORT_BIT(0x04, IP_ACTIVE_LOW, IPT_OTHER) PORT_NAME("2.2") PORT_CODE(KEYCODE_D)
	PORT_BIT(0x08, IP_ACTIVE_LOW, IPT_OTHER) PORT_NAME("2.3") PORT_CODE(KEYCODE_F)
	PORT_BIT(0x10, IP_ACTIVE_LOW, IPT_OTHER) PORT_NAME("2.4") PORT_CODE(KEYCODE_G)
	PORT_BIT(0x20, IP_ACTIVE_LOW, IPT_OTHER) PORT_NAME("2.5") PORT_CODE(KEYCODE_H)
	PORT_BIT(0x40, IP_ACTIVE_LOW, IPT_OTHER) PORT_NAME("2.6") PORT_CODE(KEYCODE_J)
	PORT_BIT(0x80, IP_ACTIVE_LOW, IPT_OTHER) PORT_NAME("2.7") PORT_CODE(KEYCODE_K)

	PORT_START("IN3")
	PORT_BIT(0x01, IP_ACTIVE_LOW, IPT_OTHER) PORT_NAME("3.0") PORT_CODE(KEYCODE_LSHIFT)
	PORT_BIT(0x02, IP_ACTIVE_LOW, IPT_OTHER) PORT_NAME("3.1") PORT_CODE(KEYCODE_Z)
	PORT_BIT(0x04, IP_ACTIVE_LOW, IPT_OTHER) PORT_NAME("3.2") PORT_CODE(KEYCODE_X)
	PORT_BIT(0x08, IP_ACTIVE_LOW, IPT_OTHER) PORT_NAME("3.3") PORT_CODE(KEYCODE_C)
	PORT_BIT(0x10, IP_ACTIVE_LOW, IPT_OTHER) PORT_NAME("3.4") PORT_CODE(KEYCODE_V)
	PORT_BIT(0x20, IP_ACTIVE_LOW, IPT_OTHER) PORT_NAME("3.5") PORT_CODE(KEYCODE_B)
	PORT_BIT(0x40, IP_ACTIVE_LOW, IPT_OTHER) PORT_NAME("3.6") PORT_CODE(KEYCODE_N)
	PORT_BIT(0x80, IP_ACTIVE_LOW, IPT_OTHER) PORT_NAME("3.7") PORT_CODE(KEYCODE_M)

	PORT_START("IN4")
	PORT_BIT(0x01, IP_ACTIVE_LOW, IPT_OTHER) PORT_NAME("4.0") PORT_CODE(KEYCODE_9)
	PORT_BIT(0x02, IP_ACTIVE_LOW, IPT_OTHER) PORT_NAME("4.1") PORT_CODE(KEYCODE_0)
	PORT_BIT(0x04, IP_ACTIVE_LOW, IPT_OTHER) PORT_NAME("4.2") PORT_CODE(KEYCODE_MINUS)
	PORT_BIT(0x08, IP_ACTIVE_LOW, IPT_OTHER) PORT_NAME("4.3") PORT_CODE(KEYCODE_EQUALS)
	PORT_BIT(0xf0, IP_ACTIVE_LOW, IPT_UNUSED)

	PORT_START("IN5")
	PORT_BIT(0x01, IP_ACTIVE_LOW, IPT_OTHER) PORT_NAME("5.0") PORT_CODE(KEYCODE_COMMA)
	PORT_BIT(0x02, IP_ACTIVE_LOW, IPT_OTHER) PORT_NAME("5.1") PORT_CODE(KEYCODE_STOP)
	PORT_BIT(0x04, IP_ACTIVE_LOW, IPT_OTHER) PORT_NAME("5.2") PORT_CODE(KEYCODE_SLASH)
	PORT_BIT(0x08, IP_ACTIVE_LOW, IPT_OTHER) PORT_NAME("5.3") PORT_CODE(KEYCODE_RSHIFT)
	PORT_BIT(0xf0, IP_ACTIVE_LOW, IPT_UNUSED)
INPUT_PORTS_END


void trocana_state::trocana(machine_config &config)
{
	TMP90841(config, m_maincpu, 16'000'000 / 2); // actually TMP90C041AN
	m_maincpu->set_addrmap(AS_PROGRAM, &trocana_state::mem_map);

	SPEAKER(config, "mono").front_center();
	OKIM6650(config, m_oki, 16'500'000 / 4).add_route(ALL_OUTPUTS, "mono", 1.0);
}

void hpierrot_state::hpierrot(machine_config &config)
{
	TMP90841(config, m_maincpu, 16_MHz_XTAL / 2); // actually TMP90C041AN
	m_maincpu->set_addrmap(AS_PROGRAM, &hpierrot_state::mem_map);

	SPEAKER(config, "mono").front_center();
	OKIM6295(config, m_oki, 16_MHz_XTAL / 16, okim6295_device::PIN7_HIGH).add_route(ALL_OUTPUTS, "mono", 1.0); // divider and pin not verified
}

void hpierrot_state::drail(machine_config &config)
{
	TMP90841(config, m_maincpu, 16_MHz_XTAL / 2); // exact model unknown as the socket was empty
	m_maincpu->set_addrmap(AS_PROGRAM, &hpierrot_state::drail_mem_map);

	nmk112_device &nmk112(NMK112(config, "nmk112", 0));
	nmk112.set_rom0_tag("oki");

	SPEAKER(config, "mono").front_center();
	OKIM6295(config, m_oki, 16_MHz_XTAL / 16, okim6295_device::PIN7_HIGH).add_route(ALL_OUTPUTS, "mono", 1.0); // divider and pin not verified
}


ROM_START( drail ) // handwritten labels
	ROM_REGION(0x10000, "maincpu", 0)
	ROM_LOAD( "1_dream rail v08c 2c0c v957220.u4", 0x00000, 0x10000, CRC(c14fae88) SHA1(f0478b563ac851372bc0b93772d89ab70ad61877) ) // 通ドリームレール V08C 2C0C V957220, 1xxxxxxxxxxxxxxx = 0xFF

	ROM_REGION(0x80000, "oki", 0)
	ROM_LOAD( "2_dream rail pcm.u11", 0x00000, 0x80000, CRC(efdc1eea) SHA1(c39fed6f97b71556b468e0872a8240fe7b6495e6) ) // ドリームレールPCM
	// empty socket 3.u10
ROM_END

ROM_START( trocana )
	ROM_REGION(0x10000, "maincpu", 0)
	ROM_LOAD( "tro1e.u12", 0x00000, 0x10000, CRC(f285043f) SHA1(6691091c1ecdab10c390db1d82c9d1d1dd0ded1f) ) // 1xxxxxxxxxxxxxxx = 0xFF

	ROM_REGION(0x80000, "oki", 0)
	ROM_LOAD( "tro2.u16",  0x00000, 0x80000, CRC(c801d8ca) SHA1(f57026f5386467c054299556dd8665e62557aa91) )
ROM_END

ROM_START( hpierrot )
	ROM_REGION(0x10000, "maincpu", 0)
	ROM_LOAD( "v8210.u44", 0x00000, 0x10000, CRC(313d5d07) SHA1(2802c88a21a311d552e8f2bd9e588ca7450f695d) ) //  1xxxxxxxxxxxxxxx = 0xFF

	ROM_REGION(0x80000, "oki", 0)
	ROM_LOAD( "sound.u27",  0x00000, 0x80000, CRC(fb6b4361) SHA1(7aaecf55efe219cb0b5eb93fee329b8e6ce0b307) )
ROM_END

} // Anonymous namespace


GAME( 1995, drail,    0, drail,    trocana, hpierrot_state, empty_init, ROT0, "NTC / NMK", "Dream Rail",    MACHINE_IS_SKELETON_MECHANICAL ) // NMK LTD, N.T.C., H07051, V39, TEST2, V07  strings
GAME( 1996, trocana,  0, trocana,  trocana, trocana_state,  empty_init, ROT0, "NTC / NMK", "Trocana",       MACHINE_IS_SKELETON_MECHANICAL ) // NMK LTD, NTC LTD, V96313 strings
GAME( 1996, hpierrot, 0, hpierrot, trocana, hpierrot_state, empty_init, ROT0, "NTC / NMK", "Happy Pierrot", MACHINE_IS_SKELETON_MECHANICAL ) // NTC LTD, NMK LTD, V96821 strings
