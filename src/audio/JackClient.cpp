/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#include <iostream>
#include <mutex> 
#include <jack/jack.h>
#include <AstUtils0.h>
#include <cstring>
#include "JackClient.h"

namespace astu {

    void CopyProcessFunc(float * in, float *out, unsigned int n)
    {
        memcpy(out, in, sizeof(float) * n);
    }

    const char *serverName = nullptr;
    jack_client_t *client = nullptr;
    jack_port_t *inputPort = nullptr;
    jack_port_t *outputPort = nullptr;
    unsigned int sampleRate = 0;
    std::mutex mtx;
    bool jackClientRunning = false;
    ProcessFunc processFunc = CopyProcessFunc;
};

int OnJackProcess(jack_nframes_t nframes, void *arg)
{
    float *in, *out;
    in = reinterpret_cast<jack_default_audio_sample_t*>(jack_port_get_buffer (astu::inputPort, nframes));
    out = reinterpret_cast<jack_default_audio_sample_t*>(jack_port_get_buffer (astu::outputPort, nframes));

    astu::processFunc(in, out, nframes);

    return 0;
}

void OnJackShutdown(void *arg)
{
    astu::mtx.lock();
    astu::jackClientRunning = false;
    astu::mtx.unlock();
}

int OnSampleRate(jack_nframes_t nframes, void *arg)
{
    astu::mtx.lock();
    astu::sampleRate = static_cast<unsigned int>(nframes);
    astu::mtx.unlock();
}

unsigned int GetSampleRate()
{
    astu::mtx.lock();
    auto result = astu::sampleRate;
    astu::mtx.unlock();

    return result;
}

bool IsJackClientTerminated()
{
    astu::mtx.lock();
    auto result = !astu::jackClientRunning;
    astu::mtx.unlock();

    return result;
}

int SetJackProcessFunction(ProcessFunc func)
{
    if (IsJackClientTerminated()) {
        SetLastError(JACK_ERROR);
        SetErrorDetails("Process function must not be set while running");            
        return GetLastError();
    }

    astu::processFunc = func;
    return NO_ERROR;
}


int InitJackClient(const char clientName[])
{
    jack_options_t options = JackNullOption;
	jack_status_t status;    

    astu::client = jack_client_open(clientName, options, &status, astu::serverName);
    if (astu::client == nullptr) {
        SetLastError(JACK_ERROR);
        if (status && JackServerFailed) {
            SetErrorDetails("Unable to connect to JACK server");            
        } else {
            std::string txt = std::string("Client open failed: ") + std::to_string(status);
            SetErrorDetails(txt.c_str());            
        }
        QuitJackClient();
        return GetLastError();
    }

    jack_set_process_callback(astu::client, OnJackProcess, 0);
    jack_on_shutdown (astu::client, OnJackShutdown, 0);
    jack_set_sample_rate_callback(astu::client, OnSampleRate, 0);

	astu::outputPort = jack_port_register(
        astu::client, "output",
        JACK_DEFAULT_AUDIO_TYPE,
        JackPortIsOutput, 0);    

    if(!astu::outputPort) {
        SetLastError(JACK_ERROR);
        SetErrorDetails("No more output ports available");            
        QuitJackClient();
        return GetLastError();
    }

    astu::inputPort = jack_port_register (
        astu::client, "input",
        JACK_DEFAULT_AUDIO_TYPE,
        JackPortIsInput, 0);

    if(!astu::inputPort) {
        SetLastError(JACK_ERROR);
        SetErrorDetails("No more input ports available");            
        QuitJackClient();
        return GetLastError();
    }

	if (jack_activate(astu::client)) {
        SetLastError(JACK_ERROR);
        SetErrorDetails("Unable to activate JACK client");            
        QuitJackClient();
        return GetLastError();
	}
    astu::jackClientRunning = true;

    return NO_ERROR;
}

void QuitJackClient()
{
    if (astu::client) {
        jack_client_close(astu::client);
        astu::client = nullptr;
    }

    astu::inputPort = nullptr;
    astu::outputPort = nullptr;
    astu::jackClientRunning = false;
}