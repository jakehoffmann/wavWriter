#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <time.h>

#define NUM_SAMPLES 500000
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

int main()
{
	
	srand(time(NULL));
	
	FILE *fPtr;
	
	int sample;
//	int rand1;
//	int frequency = 523;
	
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
	fmtChunk->sampleRate = 10000;
	fmtChunk->sigBitsPerSample = 8;
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
	dataChunk->chunkSize = fmtChunk->numberOfChannels * NUM_SAMPLES * fmtChunk->avgBytesPerSecond / 8;
	
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
	for ( int k = 0; k < NUM_SAMPLES; k++ )
	{
		
		time = ((float)k)/fmtChunk->sampleRate;
		
		//		sample = 64*sin(2*PI*frequency*time + sin(75*time))+127;
		
		sample = ( 64*sin(2*PI*523*time) +
				  64*pow(sin(2*PI*523*time),5) ) / 2 + 127;
		
		if ( ( k >= 10000 && k < 12500) )
		{
			sample = ( 64*sin(2*PI*523*time) +
					  64*pow(sin(2*PI*523*time),5) ) / 2 + 127;
		}
		
		else if ( ( k >= 12500 && k < 20000 ) || ( k >= 30000 && k < 40000 ) )
		{
			sample = ( 64*sin(2*PI*0*time) +
					  64*sin(2*PI*0*time) ) / 2 + 127;
		}
		
		else if ( ( k >= 20000 && k < 30000 )  )
		{
			sample = ( 64*sin(2*PI*523*time) +
					  64*sin(2*PI*523*time) ) / 2 + 127;
		}
		
		else if  ( k >= 50000 )
		{
			sample = ( 64*sin(2*PI*523*time) +
					  64*sin(2*PI*523*time) ) / 2 + 127;
		}	
		
		if ( sample >= 1000 || sample <= -1000 )
			sample /= 1000;
		
		fwrite( &sample, 1, 1, fPtr );
		
		printf("k: %d, time: %f, sample: %d\n", k, time, sample );
	}
	
	fwrite( slntChunk->chunkID, 1, 4, fPtr );
	fwrite( &slntChunk->chunkSize, 1, 4, fPtr);
	
	fclose(fPtr);
	
	free(riffChunk);
	free(fmtChunk);
	free(dataChunk);
	free(slntChunk);
	
	return 0;
	
}












