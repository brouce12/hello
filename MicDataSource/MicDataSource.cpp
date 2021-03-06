#include "MicDataSource.h"
#include "log.h"

// #define DUMP_PCM_DATA
using namespace std;

MicDataSource::MicDataSource(string deviceName, int rate,
							 int channel, snd_pcm_format_t dataFormart, int micProfileId, int micFormatId, float micDist)
	: deviceName(deviceName), rate(rate), channel(channel), dataFormart(dataFormart),
	  micProfileId(micProfileId), micFormatId(micFormatId), micDist(micDist), th_capture(0),
	  isRun(false), capture_handle(NULL), capture_hw_params(NULL), sArray(NULL), pfifo(NULL)
{
	this->sArray = (short *)malloc(sizeof(short) * FRAMELEN * this->channel * FIFO_BUFF_COUNT);
	this->pfifo = &this->fifo;
	FIFO_Init(this->pfifo, sArray, sizeof(short) * FRAMELEN * this->channel, FIFO_BUFF_COUNT);
	this->prepare();
}
MicDataSource::~MicDataSource()
{
	this->isRun = false;
	delete instance;
}
MicDataSource::MicDataSource(const MicDataSource &)
	: deviceName("hw:0"), rate(16000), channel(1), dataFormart(SND_PCM_FORMAT_S16_LE),
	  micProfileId(1), micFormatId(4), micDist(0.06), th_capture(0),
	  isRun(false), capture_handle(NULL), capture_hw_params(NULL), sArray(NULL), pfifo(NULL)
{
}

MicDataSource &MicDataSource::operator=(const MicDataSource &s)
{
	if (this == &s)
		return *this;
	this->deviceName = s.deviceName;
	this->rate = s.rate;
	this->channel = s.channel;
	this->dataFormart = s.dataFormart;
	this->micProfileId = s.micProfileId;
	this->micFormatId = s.micFormatId;
	this->micDist = s.micDist;
	this->th_capture = 0;
	this->isRun = s.isRun;
	this->capture_handle = s.capture_handle;
	this->capture_hw_params = s.capture_hw_params;
	this->sArray = NULL;
	this->pfifo = NULL;
	return *this;
}

//在此处初始化
MicDataSource *MicDataSource::instance = NULL;

MicDataSource *MicDataSource::getInstance(string deviceName, int rate,
										  int channel, snd_pcm_format_t dataFormart, int micProfileId, int micFormatId, float micDist)
{
	if (instance == NULL)
	{
		// lock();
		if (instance == NULL)
		{
			instance = new MicDataSource(deviceName, rate, channel, dataFormart, micProfileId, micFormatId, micDist);
		}
		// unlock();
	}
	return instance;
}

string MicDataSource::getDeviceName()
{
	return this->deviceName;
}
int MicDataSource::getRate()
{
	return this->rate;
}
int MicDataSource::getChannel()
{
	return this->channel;
}
snd_pcm_format_t MicDataSource::getDataFormart()
{
	return this->dataFormart;
}

int MicDataSource::getMicProfileId()
{
	return this->micProfileId;
}
int MicDataSource::getMicFormatId()
{
	return this->micFormatId;
}
float MicDataSource::getMicDist()
{
	return this->micDist;
}
bool MicDataSource::getDataPackage(void *data)
{
	return 1 == FIFO_GetOne(this->pfifo, data);
}
bool MicDataSource::getIsRun()
{
	return this->isRun;
}

pthread_t MicDataSource::getCaptureThreadId()
{
	return this->th_capture;
}

int MicDataSource::prepare()
{
	int err = 0;
	int dir;
	unsigned int tmpRate = this->rate;
	if ((err = snd_pcm_open(&this->capture_handle, this->deviceName.c_str(), SND_PCM_STREAM_CAPTURE, 0)) < 0)
	{
		macroFuncVargs("capture: cannot open capture audio device %s (err=%d, %s)",
					   this->deviceName.c_str(),
					   err,
					   snd_strerror(err));
		return -1;
	}
	macroFunc("capture: capture audio interface opened");

	if ((err = snd_pcm_nonblock(this->capture_handle, 0)) != 0)
	{
		macroFuncVargs("capture: cannot set block capture audio device %s (%s)",
					   this->deviceName.c_str(),
					   snd_strerror(err));
		return -2;
	}
	macroFunc("capture: capture audio interface set block");

	if ((err = snd_pcm_hw_params_malloc(&this->capture_hw_params)) < 0)
	{
		macroFuncVargs("capture: cannot allocate hardware parameter structure (%s)",
					   snd_strerror(err));
		return -3;
	}
	macroFunc("capture: this->capture_hw_params allocated");

	if ((err = snd_pcm_hw_params_any(this->capture_handle, this->capture_hw_params)) < 0)
	{
		macroFuncVargs("capture: cannot initialize hardware parameter structure (%s)",
					   snd_strerror(err));
		return -4;
	}
	macroFunc("capture: this->capture_hw_params initialized");

	if ((err = snd_pcm_hw_params_set_access(this->capture_handle, this->capture_hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0)
	{
		macroFuncVargs("capture: cannot set access type (%s)",
					   snd_strerror(err));
		return -5;
	}
	macroFunc("capture: this->capture_hw_params access setted");

	if ((err = snd_pcm_hw_params_set_format(this->capture_handle, this->capture_hw_params, this->dataFormart)) < 0)
	{
		macroFuncVargs("capture: cannot set sample format (%s)",
					   snd_strerror(err));
		return -6;
	}
	macroFunc("capture: this->capture_hw_params format setted");

	dir = 0;
	if ((err = snd_pcm_hw_params_set_rate_near(this->capture_handle, this->capture_hw_params, &tmpRate, &dir)) < 0)
	{
		macroFuncVargs("capture: cannot set sample rate (%s)",
					   snd_strerror(err));
		return -7;
	}
	macroFuncVargs("capture: this->capture_hw_params rate setted, rate is %d", tmpRate);

	if ((err = snd_pcm_hw_params_set_channels(this->capture_handle, this->capture_hw_params, this->channel)) < 0)
	{
		macroFuncVargs("capture: cannot set channel count (%s)",
					   snd_strerror(err));
		return -8;
	}

	macroFunc("capture: this->capture_hw_params channels setted");

	if ((err = snd_pcm_hw_params_set_buffer_size(this->capture_handle, this->capture_hw_params, FRAMELEN * 2)) < 0)
	{
		macroFuncVargs("capture: cannot set buffer size (%s)",
					   snd_strerror(err));
		return -9;
	}
	macroFunc("capture: this->capture_hw_params buffer size setted");

	dir = 0;
	if ((err = snd_pcm_hw_params_set_period_size(this->capture_handle, this->capture_hw_params, FRAMELEN, dir)) < 0)
	{
		macroFuncVargs("capture: cannot set period size (%s)",
					   snd_strerror(err));
		return -10;
	}
	macroFunc("capture: this->capture_hw_params period size setted");

	int val;
	snd_pcm_hw_params_get_buffer_size(this->capture_hw_params,
									  (snd_pcm_uframes_t *)&val);
	macroFuncVargs("capture: buffer size = %d frames", val);

	if ((err = snd_pcm_hw_params(this->capture_handle, this->capture_hw_params)) < 0)
	{
		macroFuncVargs("capture: cannot set parameters (%s)",
					   snd_strerror(err));
		return -11;
	}

	macroFunc("capture: this->capture_hw_params setted");

	snd_pcm_hw_params_free(this->capture_hw_params);
	this->capture_hw_params = NULL;

	macroFunc("capture: this->capture_hw_params freed");

	if ((err = snd_pcm_prepare(this->capture_handle)) < 0)
	{
		macroFuncVargs("capture: cannot prepare audio interface for use (%s)",
					   snd_strerror(err));
		return -12;
	}

	macroFunc("capture: audio interface prepared");
	return 0;
}

void MicDataSource::run()
{
	if (!this->isRun)
	{
		pthread_create(&this->th_capture, NULL, capture_process, (void *)this);
	}
}

void MicDataSource::stop()
{
	this->isRun = false;
}

void *MicDataSource::capture_process(void *p)
{
	// pthread_detach(pthread_self());
	MicDataSource *micDataSource = (MicDataSource *)p;
	micDataSource->isRun = true;
	int err = 0;
	static int flag = 0;

	short *inputMicData16s = NULL;
	inputMicData16s = (short *)malloc(sizeof(short) * FRAMELEN * micDataSource->channel);

#ifdef DUMP_PCM_DATA
	FILE *dumpReadOutput = NULL;
	if ((dumpReadOutput = fopen("./dump_raw.pcm", "wb")) == NULL)
	{
		macroFunc("wakeup: cannot open dump_raw file for wakeup");
		exit(1);
	}
#endif
	macroFunc("capture start. ");

	while (micDataSource->isRun)
	{
		if ((err = snd_pcm_readi(micDataSource->capture_handle, inputMicData16s, FRAMELEN)) != FRAMELEN)
		{
			// read from audio interface failed -77, (File descriptor in bad state)
			macroFuncVargs("capture: read from audio interface failed %d, (%s)",
						   err, snd_strerror(err));
			if (-EPIPE == err || err == -77)
			{
				if ((err = snd_pcm_prepare(micDataSource->capture_handle)) < 0)
				{
					macroFuncVargs("capture: cannot prepare audio interface for use (%s)",
								   snd_strerror(err));
					break;
				}
				macroFunc("capture: audio interface prepared");
				continue;
				macroFunc("capture: audio readi continue");
			}
			else if (-19 == err)
			{
				macroFunc("capture: audio readi error, Exit(-12)");
				break;
			}
			else
			{
				macroFunc("capture: audio readi error, Exit(-13)");
				break;
			}
		}

		// if (micDataSource->status == 1)
		// { //save to asr
		// 	macroFunc("capture: after wakeup, prepare save audio file to asr.");
		// 	// struct timeval tv;
		// 	// gettimeofday(&tv,NULL);
		// 	// char *asrFile = "asrfile";
		// 	if (0 == flag)
		// 	{
		// 		FILE *fpw;
		// 		fpw = fopen(ASRFILE, "wb");
		// 		if (!fpw)
		// 		{
		// 			macroFunc("capture: fopen failed.");
		// 			exit(1);
		// 		}
		// 		flag=1;
		// 	}

		// 	fwrite(inputMicData16s, sizeof(short), FRAMELEN, fpw);
		// 	fflush(fpw);
		// 	// if (0 == flag)
		// 	// {
		// 	// 	fclose(fpw);
		// 	// }

		// 	continue;
		// }

		if (0 == FIFO_AddOne(micDataSource->pfifo, inputMicData16s))
		{
			macroFunc("capture: buf_write fail, buffer is full.");
		}

#ifdef DUMP_PCM_DATA
		fwrite(inputMicData16s, sizeof(short), MicDataSource::FRAMELEN * micDataSource->getChannel(), dumpReadOutput);
		fflush(dumpReadOutput);
#endif
	}
	if (NULL != inputMicData16s)
	{
		free(inputMicData16s);
		inputMicData16s = NULL;
	}

	if (NULL != micDataSource->capture_hw_params)
	{
		snd_pcm_hw_params_free(micDataSource->capture_hw_params);
		micDataSource->capture_hw_params = NULL;
	}
	if (NULL != micDataSource->capture_handle)
	{
		snd_pcm_drain(micDataSource->capture_handle);
		snd_pcm_close(micDataSource->capture_handle);
	}
	if (NULL != micDataSource->sArray)
	{
		free(micDataSource->sArray);
		micDataSource->sArray = NULL;
	}
	return NULL;
}

int MicDataSource::emptyFifo()
{
	return FIFO_DelAdd(pfifo, 6);
}