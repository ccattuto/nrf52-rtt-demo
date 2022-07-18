#include <common.h>
#include <timer.h>
#include <string.h>
#include <SEGGER_RTT.h>
#include <rtt.h>

static uint8_t rtt_read_buf[BUFFER_SIZE_DOWN];
static uint32_t cmd_counter = 0;

void exec_command(const char *cmd)
{
	if (strcmp(cmd, "led2") == 0) {
		nrf_gpio_pin_toggle(LED2_PIN);
		rtt_printf("[%d] toggled led 2\r\n", ++cmd_counter);
	} else if (strcmp(cmd, "led3") == 0) {
		nrf_gpio_pin_toggle(LED3_PIN);
		rtt_printf("[%d] toggled led 3\r\n", ++cmd_counter);	
	} else if (strcmp(cmd, "led4") == 0) {
		nrf_gpio_pin_toggle(LED4_PIN);
		rtt_printf("[%d] toggled led 4\r\n", ++cmd_counter);	
	} else
		rtt_printf("unknown command: %s\r\n", cmd);
} 

int main(void)
{
	uint8_t rtt_line_buf_len;

    /* enable cache */
    NRF_NVMC->ICACHECNF = (NVMC_ICACHECNF_CACHEEN_Enabled << NVMC_ICACHECNF_CACHEEN_Pos);

    /* start DC-DC converter */
    NRF_POWER->DCDCEN = (POWER_DCDCEN_DCDCEN_Enabled << POWER_DCDCEN_DCDCEN_Pos);

    /* start HF oscillator */
    NRF_CLOCK->TASKS_HFCLKSTART = 1;

    /* start 32kHz crystal oscillator */
    NRF_CLOCK->LFCLKSRC = (CLOCK_LFCLKSRC_SRC_Xtal << CLOCK_LFCLKSRC_SRC_Pos);
    NRF_CLOCK->EVENTS_LFCLKSTARTED = 0;
    NRF_CLOCK->TASKS_LFCLKSTART = 1;
    while (NRF_CLOCK->EVENTS_LFCLKSTARTED == 0);

	/* enable LED output */
	nrf_gpio_cfg_output(LED1_PIN);
	nrf_gpio_cfg_output(LED2_PIN);
	nrf_gpio_cfg_output(LED3_PIN);
	nrf_gpio_cfg_output(LED4_PIN);

	/* switch them all all */
	nrf_gpio_pin_set(LED1_PIN);
	nrf_gpio_pin_set(LED2_PIN);
	nrf_gpio_pin_set(LED3_PIN);
	nrf_gpio_pin_set(LED4_PIN);

	/* enabled button input pin */
	nrf_gpio_cfg_input(BUTTON_PIN, NRF_GPIO_PIN_PULLUP);

	/* start timer */
	timer_init();

    // set up RTT
    SEGGER_RTT_Init();
    //SEGGER_RTT_SetFlagsUpBuffer(0, SEGGER_RTT_MODE_BLOCK_IF_FIFO_FULL);
	rtt_printf("\r\nThis is a demo nRF52 firmware controllable via RTT terminal.\r\n");
	rtt_printf("Type \"led2\" / \"led3\" / \"led4\" followed by ENTER to toggle LEDs on the nRF52-DK board.\r\n");
	rtt_printf("Clicking button 1 is announced over RTT.\r\n\r\n");

	// main loop
	while (1)
	{
        nrf_gpio_pin_toggle(LED1_PIN);
		timer_wait(100);

		// read from RTT
        rtt_line_buf_len = SEGGER_RTT_Read(0, rtt_read_buf, BUFFER_SIZE_DOWN);
        if (rtt_line_buf_len > 0) {
            rtt_read_buf[rtt_line_buf_len-1] = 0;
            rtt_read_buf[strcspn((char *) rtt_read_buf, "\r\n")] = 0; // strip trailing CR/LF
			if (rtt_read_buf[0] == 0)
				continue;

            exec_command((const char *) rtt_read_buf);
        }

		// check button state
 		if (!nrf_gpio_pin_read(BUTTON_PIN))
			rtt_printf("button 1 is pressed\r\n");			
	}
}
