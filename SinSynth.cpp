#include "SinSynth.h"

#pragma mark SinSynth Methods

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//COMPONENT_ENTRY(SinSynth)
AUDIOCOMPONENT_ENTRY(AUMusicDeviceFactory, SinSynth)

static const AudioUnitParameterID kGlobalVolumeParam = 0;
static const CFStringRef kGlobalVolumeName = CFSTR("global volume");

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	SinSynth::SinSynth
//
// This synth has No inputs, One output
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SinSynth::SinSynth(AudioUnit inComponentInstance)
	: AUMonotimbralInstrumentBase(inComponentInstance, 0, 1)
{
	CreateElements();
	
	Globals()->UseIndexedParameters(1); // we're only defining one param
	Globals()->SetParameter (kGlobalVolumeParam, 1.0);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	SinSynth::~SinSynth
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SinSynth::~SinSynth()
{
}


void SinSynth::Cleanup()
{
#if DEBUG_PRINT
	printf("SinSynth::Cleanup\n");
#endif
}

OSStatus SinSynth::Initialize()
{	
#if DEBUG_PRINT
	printf("->SinSynth::Initialize\n");
#endif
	AUMonotimbralInstrumentBase::Initialize();
	
//	SetNotes(kNumNotes, kMaxActiveNotes, mTestNotes, sizeof(TestNote));
#if DEBUG_PRINT
	printf("<-SinSynth::Initialize\n");
#endif
	
	return noErr;
}

AUElement* SinSynth::CreateElement(	AudioUnitScope					scope,
									AudioUnitElement				element)
{
	switch (scope)
	{
		case kAudioUnitScope_Group :
			return new SynthGroupElement(this, element, new MidiControls);
		case kAudioUnitScope_Part :
			return new SynthPartElement(this, element);
		default :
			return AUBase::CreateElement(scope, element);
	}
}

OSStatus			SinSynth::GetParameterInfo(		AudioUnitScope					inScope,
														AudioUnitParameterID			inParameterID,
														AudioUnitParameterInfo &		outParameterInfo)
{
	if (inParameterID != kGlobalVolumeParam) return kAudioUnitErr_InvalidParameter;
	if (inScope != kAudioUnitScope_Global) return kAudioUnitErr_InvalidScope;

	outParameterInfo.flags = SetAudioUnitParameterDisplayType (0, kAudioUnitParameterFlag_DisplaySquareRoot);
    outParameterInfo.flags += kAudioUnitParameterFlag_IsWritable;
	outParameterInfo.flags += kAudioUnitParameterFlag_IsReadable;

	AUBase::FillInParameterName (outParameterInfo, kGlobalVolumeName, false);
	outParameterInfo.unit = kAudioUnitParameterUnit_LinearGain;
	outParameterInfo.minValue = 0;
	outParameterInfo.maxValue = 1.0;
	outParameterInfo.defaultValue = 1.0;
	return noErr;
}
