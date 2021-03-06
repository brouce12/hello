#include "LedService.h"
#include "log.h"
#include "serialControl.h"

LedService::LedService(int pin, int default_mode, int sleep_ms):   
	m_pin(pin), m_isrun(false), m_sleep_ms(sleep_ms), m_light_status(default_mode) {

	wiringPiSetup();
	pinMode(m_pin, OUTPUT) ;
    digitalWrite (10, default_mode) ;

	pthread_cond_init(&this->m_Cond, NULL);
    pthread_mutex_init(&this->m_Mutex, NULL);
	m_start_time.tv_sec =0;
	m_start_time.tv_usec=0;
}
void LedService::onWakeup(WakeupEvent *wakeupEvent){
	pthread_mutex_lock(&this->m_Mutex);
	gettimeofday(&m_start_time,NULL);
	pthread_mutex_unlock(&this->m_Mutex);
	lightUp();
	
	bool isUnlockSuccess = serialWrite(GATE_LOCK_SERIAL_PORT, GATE_LOCK_SERIAL_RATE, "2334");
	if(!isUnlockSuccess)
		serialWrite(GATE_LOCK_SERIAL_PORT, GATE_LOCK_SERIAL_RATE, "2334");
}
LedService::~LedService() {

}
void LedService::lightUp(){
	m_light_status = HIGH;
	digitalPull (m_pin, HIGH);
}
void LedService::blackOut(){
	m_light_status = LOW;
	digitalPull (m_pin, LOW);
}

void LedService::digitalPull(int pin, int mode)
{
    if(HIGH == mode){
	    pinMode(pin, OUTPUT);
        digitalWrite (pin, HIGH) ;	// On
    }else  if(LOW == mode){
	    // pinMode(pin, OUTPUT);
        // digitalWrite (pin, LOW) ;	// Off
	    pinMode(pin, INPUT);
    }
}

void LedService::run(){
	if (!this->m_isrun)
   {
	   pthread_create(&this->m_th, NULL, m_Process, (void *)this);
   }
}
void LedService::stop(){
	this->m_isrun = false;
}
bool LedService::getIsRun(){
	return this->m_isrun;
}
void * LedService::m_Process(void *p){
	pthread_detach(pthread_self());
	LedService *ledService = (LedService *)p;
    ledService->m_isrun = true;
	struct timeval cur_time;
	  while (ledService->m_isrun)
    {
		gettimeofday(&cur_time,NULL);	
		pthread_mutex_lock(&ledService->m_Mutex);
		float Timeuse;
		Timeuse = 1000000*(cur_time.tv_sec - ledService->m_start_time.tv_sec) + (cur_time.tv_usec - ledService->m_start_time.tv_usec);
		Timeuse /= 1000;
		pthread_mutex_unlock(&ledService->m_Mutex);
		if(Timeuse >= ledService->m_sleep_ms){//1500ms
			if(ledService->m_light_status == HIGH){
				
			}
			ledService->blackOut();
		}
		usleep(5000);
    }  
}


