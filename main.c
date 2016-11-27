#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <time.h>

#define PI 4*atan(1)

struct riffChunk
{
	char chunkID[4];
	uint32_t chunkSize;
	char riffType[4];
};

typedef struct riffChunk RiffChunk;
typedef struct riffChunk* RiffPtr;

struct fmtChunk
{
	char chunkID[4];
	uint32_t chunkSize;
	uint16_t compressionCode;
	uint16_t numberOfChannels;
	uint32_t sampleRate;
	uint32_t avgBytesPerSecond;
	uint16_t blockAlign;
	uint16_t sigBitsPerSample;
};

typedef struct fmtChunk FmtChunk;
typedef struct fmtChunk* FmtPtr;

struct dataChunk
{
	char chunkID[4];
	uint32_t chunkSize;
};

typedef struct dataChunk DataChunk;
typedef struct dataChunk* DataChunkPtr;

struct slntChunk
{
	char chunkID[4];
	uint32_t chunkSize;
	uint32_t numOfSilentSamples;
};

typedef struct slntChunk SlntChunk;
typedef struct slntChunk* SlntPtr;

int calculateSample(float freq, float time);

int main()
{
	
	srand(time(NULL));
	
	FILE *fPtr;
	
	int16_t sample;

	int audioLength = 3; // seconds
	int sampleRate = 44100; // Hz
	int numSamples = audioLength * sampleRate;
	
	// Create riff chunk
	RiffPtr riffChunk = (RiffPtr) malloc( sizeof(RiffChunk) );
	
	// Initialize riff chunk
	strcpy( riffChunk->chunkID, "RIFF" );
	strcpy( riffChunk->riffType, "WAVE" );
	
	// Create format chunk
	FmtPtr fmtChunk = (FmtPtr) malloc( sizeof(FmtChunk) );
	
	// Initialize the format chunk data
	strcpy( fmtChunk->chunkID, "fmt " );
	fmtChunk->chunkSize = 16;
	fmtChunk->compressionCode = 1;
	fmtChunk->numberOfChannels = 1;
	fmtChunk->sampleRate = sampleRate;
	fmtChunk->sigBitsPerSample = 16;         
	fmtChunk->blockAlign = fmtChunk->sigBitsPerSample *	fmtChunk->numberOfChannels / 8;
	fmtChunk->avgBytesPerSecond = fmtChunk->sampleRate * fmtChunk->blockAlign;
	
	// Create data chunk
	DataChunkPtr dataChunk = (DataChunkPtr) malloc( sizeof(DataChunk) );
	
	// Initialize the data chunk 
	strcpy( dataChunk->chunkID, "data" );
	
	// Create silent (slnt) chunk
	SlntPtr slntChunk = (SlntPtr) malloc( sizeof(SlntChunk) );
	
	// Initialize the silent chunk
	strcpy( slntChunk->chunkID, "slnt" );
	slntChunk->chunkSize = 4;
	
	// Calculate the chunk sizes and write them to their respective positions
	dataChunk->chunkSize = fmtChunk->numberOfChannels * numSamples * fmtChunk->avgBytesPerSecond / 8;
	
	// Write the data to file (binary)
	
	fPtr = fopen( "myWav.wav", "wb" );
	if ( fPtr == NULL ) 
		return -1;
	
	fwrite( riffChunk->chunkID, 1, 4, fPtr);
	fwrite( &riffChunk->chunkSize, 1, 4, fPtr);
	fwrite( riffChunk->riffType, 1, 4, fPtr);
	
	fwrite( fmtChunk->chunkID, 1, 4, fPtr);
	fwrite( &fmtChunk->chunkSize, 1, 4, fPtr);
	fwrite( &fmtChunk->compressionCode, 1, 2, fPtr);
	fwrite( &fmtChunk->numberOfChannels, 1, 2, fPtr);
	fwrite( &fmtChunk->sampleRate, 1, 4, fPtr);
	fwrite( &fmtChunk->avgBytesPerSecond, 1, 4, fPtr);
	fwrite( &fmtChunk->blockAlign, 1, 2, fPtr);
	fwrite( &fmtChunk->sigBitsPerSample, 1, 2, fPtr);
	
	fwrite( dataChunk->chunkID, 1, 4, fPtr);
	fwrite( &dataChunk->chunkSize, 1, 4, fPtr);
	
	
	printf( "%d", fmtChunk->avgBytesPerSecond ); 
	
	float time;	
	// Generate and write the sound data to file
	for ( int k = 0; k < numSamples; k++ )
	{
		
		time = ((float)k)/fmtChunk->sampleRate;
		
		// C major chord.
	    //sample = (calculateSample(164, (float)k) + calculateSample(131, (float)k) + calculateSample(196, (float)k))/3;

		// C note alone
		sample = calculateSample(131, (float)k);
		
		fwrite( &sample, 1, sizeof(sample), fPtr );
		
		printf("k: %d, time: %f, sample: %d\n", k, time, sample );
	}
	
	fwrite( slntChunk->chunkID, 1, 4, fPtr );
	fwrite( &slntChunk->chunkSize, 1, 4, fPtr);
	
	fclose(fPtr);
	
	free(riffChunk);
	free(fmtChunk);
	free(dataChunk);
	free(slntChunk);
	
	printf("done!\n");
	
	return 0;
	
}

// calculate the sample using some waveform
int calculateSample(float freq, float time)
{
	int volume = 100;

	// sine waves
	//return (int)32767*volume/100*sin(2*PI/44100*freq*time);

	// sawtooth waves
	return (int)32767*volume/100*(remainder(sin(2*PI/44100*freq*time),1));
	
	// square waves
	//return 32767*volume/100*(sin(2*PI/44100*freq*time) > 0 ? 1 : ((sin(2*PI/44100*freq*time) < 0) ? -1 : 0));  
}












