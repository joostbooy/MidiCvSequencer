#include "system.h"
#include "micros.h"
#include "timer.h"
#include "uart.h"
#include "matrix.h"
#include "display.h"
#include "adc.h"
#include "usb.h"
#include "disk.h"
#include "debug.h"

#include "settings.h"
#include "engine.h"
#include "ui.h"

extern "C" {

	void NMI_Handler() { }
	void HardFault_Handler() { while (1); }
	void MemManage_Handler() { while (1); }
	void BusFault_Handler() { while (1); }
	void UsageFault_Handler() { while (1); }
	void SVC_Handler() { }
	void DebugMon_Handler() { }
	void PendSV_Handler() { }
	void SysTick_Handler(void) { }


	// CLOCK_ISR_FREQ (8kHz)
	void TIM2_IRQHandler(void) {
		if (!(TIM2->SR & TIM_IT_UPDATE)) {
			return;
		}
		TIM2->SR = ~TIM_IT_UPDATE;
		engine.tick();
	}

	// 2kHz
	void TIM3_IRQHandler(void) {
		if (!(TIM3->SR & TIM_IT_UPDATE)) {
			return;
		}
		TIM3->SR = ~TIM_IT_UPDATE;
		engine.update();
	}

	// 1Khz
	void TIM4_IRQHandler(void) {
		if (!(TIM4->SR & TIM_IT_UPDATE)) {
			return;
		}
		TIM4->SR = ~TIM_IT_UPDATE;
		ui.poll();
	}

} //extern "C"


int main(void)
{
	sys.init();
	micros.init();	// must be called before adc, display & sdio init
	debug.init();
	matrix.init();
	display.init();
	uart.init();
	adc.init();
	dac.init();
	gateIO.init();
	usb.init();
	disk.init();
	disk.mount();

	settings.init();	// before ui & engine
	ui.init();
	engine.init();

	timer.start_2(CLOCK_ISR_FREQ);
	timer.start_3(2000);
	timer.start_4(1000);

	while (1) {
		ui.process();
		engine.process();
	}
}
