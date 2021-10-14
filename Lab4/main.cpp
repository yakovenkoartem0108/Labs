#include "gpioaregisters.hpp" //for GPIOC
#include "gpiocregisters.hpp" //for GPIOC
#include "rccregisters.hpp"   //for RCC


std::uint32_t SystemCoreClock = 16'000'000U;

extern "C" {
int __low_level_init(void)
{
  //Switch on external 16 MHz oscillator
  RCC::CR::HSION::On::Set();
  while (RCC::CR::HSIRDY::NotReady::IsSet())
  {

  }
  //Switch system clock on external oscillator
  RCC::CFGR::SW::Hsi::Set();
  while (!RCC::CFGR::SWS::Hsi::IsSet())
  {

  }

  RCC::APB2ENR::SYSCFGEN::Enable::Set();

  return 1;
}
}
void delay(int cycles)
{
 for(int i=0; i < cycles; ++i)
 {
   asm volatile ("");
 }
}
struct Leds
      {
        static void Blink()
        {
           uint32_t* ptrPcOdr = reinterpret_cast<uint32_t*>(0x40020814);
           uint32_t* ptrPaOdr = reinterpret_cast<uint32_t*>(0x40020014);
          *ptrPcOdr ^=  static_cast<uint32_t> ((1 << 5) | (1 << 8) | (1 << 9)); 
          *ptrPaOdr ^=  static_cast<uint32_t>(1 << 5); 
          delay(1000000);
          *ptrPcOdr ^=static_cast<uint32_t>((1 << 5) | (1 << 8) | (1 << 9));
          *ptrPaOdr ^=static_cast<uint32_t>(1 << 5);
          delay(1000000);
       }
      };

int main()
{
  RCC::AHB1ENR::GPIOAEN::Enable::Set();
  GPIOA::MODER::MODER5::Output::Set();
  RCC::AHB1ENR::GPIOCEN::Enable::Set();
  GPIOC::MODER::MODER5::Output::Set();
  GPIOC::MODER::MODER8::Output::Set();
  GPIOC::MODER::MODER9::Output::Set();
 
      for(;;)
       {   
        Leds::Blink();
       }   
      
  return 1;
}
