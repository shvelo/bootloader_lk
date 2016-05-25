/* Copyright (c) 2012, The Linux Foundation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials provided
 *       with the distribution.
 *     * Neither the name of The Linux Foundation nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <stdint.h>
#include <msm_panel.h>
#include <mipi_dsi.h>
#include <sys/types.h>
#include <err.h>
#include <reg.h>
#include <platform/iomap.h>
#include <mdp4.h>


//static const unsigned char
/*
	<DSIInitSequence>
		05 11 00
		ff 78
		15 3A 70
		15 53 2C
		15 51 80
		15 55 01
		15 3A 70
		05 29 00
	</DSIInitSequence>
*/

static const unsigned char race_cmd_cmds[7][4] = {
	{0x11, 0x00, 0x05, 0x80},	/* DTYPE_DCS_WRITE */	//exit sleep
	{0x78, 0x00, 0xff, 0x80},	//???
	{0x3A, 0x70, 0x15, 0x80},	/* DTYPE_DCS_WRITE1 */	//set pixel format
	{0x53, 0x2C, 0x15, 0x80},	/* DTYPE_DCS_WRITE1 */	//control display
	{0x51, 0x80, 0x15, 0x80},	/* DTYPE_DCS_WRITE1 */	//lcm brightness
	{0x55, 0x01, 0x15, 0x80},	/* DTYPE_DCS_WRITE1 */	//cabc on
	{0x3A, 0x70, 0x15, 0x80},	/* DTYPE_DCS_WRITE1 */	//set pixel format
	{0x29, 0x00, 0x05, 0x80}	/* DTYPE_DCS_WRITE */	//display on
};

static struct mipi_dsi_cmd race_cmd_mode_cmds[] = {
	{sizeof(race_cmd_cmds[0]), (char *)race_cmd_cmds[0]},
	{sizeof(race_cmd_cmds[1]), (char *)race_cmd_cmds[1]},
	{sizeof(race_cmd_cmds[2]), (char *)race_cmd_cmds[2]},
	{sizeof(race_cmd_cmds[3]), (char *)race_cmd_cmds[3]},
	{sizeof(race_cmd_cmds[4]), (char *)race_cmd_cmds[4]},
	{sizeof(race_cmd_cmds[5]), (char *)race_cmd_cmds[5]},
	{sizeof(race_cmd_cmds[6]), (char *)race_cmd_cmds[6]},
	{sizeof(race_cmd_cmds[7]), (char *)race_cmd_cmds[7]},
};


int mipi_race_cmd_config(void *pdata)
{
	int ret = NO_ERROR;

	/* 2 Lanes -- Enables Data Lane0, 1 */
	unsigned char lane_en = 3;
	unsigned long low_pwr_stop_mode = 0;

	/* Needed or else will have blank line at top of display */
	unsigned char eof_bllp_pwr = 0x8;

	unsigned char interleav = 0;
	struct lcdc_panel_info *lcdc = NULL;
	struct msm_panel_info *pinfo = (struct msm_panel_info *)pdata;

	if (pinfo == NULL)
		return ERR_INVALID_ARGS;

	lcdc =  &(pinfo->lcdc);
	if (lcdc == NULL)
		return ERR_INVALID_ARGS;

	/*ret = mipi_dsi_video_mode_config((pinfo->xres + lcdc->xres_pad),
			(pinfo->yres + lcdc->yres_pad),
			(pinfo->xres),
			(pinfo->yres),
			(lcdc->h_front_porch),
			(lcdc->h_back_porch + lcdc->h_pulse_width),
			(lcdc->v_front_porch),
			(lcdc->v_back_porch + lcdc->v_pulse_width),
			(lcdc->h_pulse_width),
			(lcdc->v_pulse_width),
			pinfo->mipi.dst_format,
			pinfo->mipi.traffic_mode,
			lane_en,
			low_pwr_stop_mode,
			eof_bllp_pwr,
			interleav);*/

	ret = mipi_dsi_cmd_mode_config((pinfo->xres),
			(pinfo->yres),
			(pinfo->xres),
			(pinfo->yres),
			pinfo->mipi.dst_format,
			pinfo->mipi.traffic_mode);

	return ret;
}

int mipi_race_cmd_on()
{
	int ret = NO_ERROR;
	return ret;
}

int mipi_race_cmd_off()
{
	int ret = NO_ERROR;
	return ret;
}

static struct mipi_dsi_phy_ctrl dsi_video_mode_phy_db = {
	/* 480*800, RGB888, 2 Lane 60 fps video mode */
	/* regulator */
	{0x02, 0x08, 0x05, 0x00, 0x20},
	/* timing */
	{0x67, 0x16, 0x0D, 0x00,
	0x38, 0x3C, 0x12, 0x19, 0x18,
	0x03, 0x03, 0xA0},
	/* phy ctrl */
	{0x5F, 0x00, 0x00, 0x10},
	/* strength */
	{0xFF, 0x00, 0x06, 0x00},
	/* pll control */
	{0x00,
	0x25, 0x30, 0xC2,
	0x00, 0x30, 0x0C, 0x62,
	0x41, 0x01, 0x01,
	0x00, 0x00, 0x00, 0x00, 0x02,
	0x00, 0x20, 0x00, 0x01/*, 0x00*/},
};

void mipi_race_cmd_init(struct msm_panel_info *pinfo)
{
	if (!pinfo)
		return;

	pinfo->xres = 480;
	pinfo->yres = 800;
	pinfo->lcdc.xres_pad = 0;
	pinfo->lcdc.yres_pad = 0;

	pinfo->type = MIPI_CMD_PANEL;
	pinfo->wait_cycle = 0;
	pinfo->bpp = 24;
	pinfo->lcdc.h_back_porch = 44;
	pinfo->lcdc.h_front_porch = 45;
	pinfo->lcdc.h_pulse_width = 4;
	pinfo->lcdc.v_back_porch = 14;
	pinfo->lcdc.v_front_porch = 14;
	pinfo->lcdc.v_pulse_width = 1;
	pinfo->lcdc.border_clr = 0;
	pinfo->lcdc.underflow_clr = 0xf0;
	pinfo->lcdc.hsync_skew = 0;
	pinfo->clk_rate = 400000000;//343848960;

	pinfo->mipi.mode = DSI_CMD_MODE;
	pinfo->mipi.pulse_mode_hsa_he = FALSE;
	pinfo->mipi.hfp_power_stop = FALSE;
	pinfo->mipi.hbp_power_stop = FALSE;
	pinfo->mipi.hsa_power_stop = FALSE;
	pinfo->mipi.eof_bllp_power_stop = TRUE;
	pinfo->mipi.bllp_power_stop = TRUE;
	pinfo->mipi.traffic_mode = DSI_NON_BURST_SYNCH_EVENT;
	pinfo->mipi.dst_format = DSI_CMD_DST_FORMAT_RGB888;
	pinfo->mipi.vc = 0;
	pinfo->mipi.rgb_swap = DSI_RGB_SWAP_RGB;
	pinfo->mipi.data_lane0 = TRUE;
	pinfo->mipi.data_lane1 = TRUE;
	pinfo->mipi.data_lane2 = FALSE;
	pinfo->mipi.data_lane3 = FALSE;
	pinfo->mipi.dlane_swap = FALSE;
	pinfo->mipi.t_clk_post = 0x0F;
	pinfo->mipi.t_clk_pre = 0x08;
	pinfo->mipi.stream = 0;
	pinfo->mipi.mdp_trigger = DSI_CMD_TRIGGER_NONE;
	pinfo->mipi.dma_trigger = DSI_CMD_TRIGGER_SW;
	pinfo->mipi.frame_rate = 60;
	pinfo->mipi.dsi_phy_db = &dsi_video_mode_phy_db;
	pinfo->mipi.tx_eot_append = TRUE;

	pinfo->mipi.num_of_lanes = 2;
	pinfo->mipi.panel_cmds = race_cmd_mode_cmds;
	pinfo->mipi.num_of_panel_cmds =
				 ARRAY_SIZE(race_cmd_mode_cmds);

	pinfo->on = mipi_race_cmd_on;
	pinfo->off = mipi_race_cmd_off;
	pinfo->config = mipi_race_cmd_config;

	return;
}
