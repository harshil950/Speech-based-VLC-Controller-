// vlc_controller.cpp : Defines the entry point for the console application.
//

#include "Windows.h"
#include "mmsystem.h"
#include "stdafx.h"
#include "stdio.h"
#include "conio.h"
#include "string.h"
#include "math.h"
#include "stdlib.h"
#include "limits.h"
#include "float.h"
#include "time.h"
#include "shellapi.h"

#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "winmm.lib")

#define SAMPLERATE 16000
#define CHANNELS 1
#define RESOLUTION 16
#define COMMAND_DURATION 8
#define WORD_DURATION 4

// To generate universe
#define NO_OF_WORDS 8
// #define TRAINING_SIZE_PER_WORD 25
static int  training_size_per_word = 1;
#define DC_SHIFT_THRESHOLD 100
#define NORMALIZATION_FACTOR 5000
#define MAX_NO_OF_SAMPLES 50000
#define FRAME_SIZE 320  // SAME AS N
#define SLIDE_LENGTH 80
#define CEPSTRAL_COEFF_PER_FRAME 12 // SAME AS P
#define MAX_WORD_LENGTH 20
static char *dictionary[MAX_WORD_LENGTH] = {"close", "down", "movies", "open", "up", "vlc", "volume", "window"};


// For LBG
#define MAX_UNIVERSE_SIZE 50000
#define MAX_CODEBOOK_SIZE 32
#define INT_MAX 2147483647
static long double tokhura_weights[CEPSTRAL_COEFF_PER_FRAME] = {1.0, 3.0, 7.0, 13.0, 19.0, 22.0, 25.0, 33.0, 42.0, 50.0, 56.0, 61.0}; // predefined tokhura's weights.


// For HMM
#define T 160
#define M 32
#define N 5
#define MAX_HMM_ITERATIONS 100
static long double alpha[T+1][N+1], beta[T+1][N+1], gamma[T+1][N+1], delta[T+1][N+1], psi[T+1][N+1], xi[T+1][N+1][N+1];
static long double A[N+1][N+1], B[N+1][M+1], pi[N+1];
static long double p_star;
static int observation[T+1], q_star[T+1];
static long double prob_of_O_given_lambda;
static int observation_seq_len;


// For testing
#define TESTING_FILE_START_NO 26
#define TESTING_FILE_END_NO 30
#define TOTAL_TESTING_SIZE 40.0

// Live testing
#define CLIP 5000				//Max value after normalizing
#define FS 320					//Frame Size
#define START_CHECKPOINT 20
#define END_CHECKPOINT 30
#define START_CONSECUTIVE_FRAMES 20
#define END_CONSECUTIVE_FRAMES 35
static long double energy[2000]={0};
static int no_of_frames;
static int start_frame;
static int end_frame;
static int *predicted_words;
static int words=0;

// For trainable module
static char *recorded_file = "recorded_data.txt";
static char *normalized_recorded_file = "normalized_recorded_data.txt";
static char *trimmed_file = "word_1.txt";
static char *normalized_trimmed_file = "normalized_word_1.txt";
static char *trainable_module_directory = "trainable_module";


static FILE *fp_log;

/*
	Start recording for particular duration length
*/
static void  StartRecord(short int *data,int length)
{
    printf("Start Recording...\n");
	fprintf(fp_log, "Start Recording...\n");
	HWAVEIN hWaveIn;
	WAVEHDR WaveInHdr;

	WAVEFORMATEX pFormat;
	pFormat.wFormatTag=WAVE_FORMAT_PCM;                         // simple, uncompressed format
	pFormat.nChannels=CHANNELS;                                 //  1=mono, 2=stereo
	pFormat.nSamplesPerSec=SAMPLERATE;                          // 16000
	pFormat.nAvgBytesPerSec=SAMPLERATE * 2;                     // = nSamplesPerSec * n.Channels * wBitsPerSample/8
	pFormat.wBitsPerSample=RESOLUTION;                          //  16 for high quality, 8 for telephone-grade
	pFormat.nBlockAlign=CHANNELS * RESOLUTION / 8;              // = n.Channels * wBitsPerSample/8
	pFormat.cbSize=0;

	waveInOpen(&hWaveIn, WAVE_MAPPER,&pFormat,0L, 0L, WAVE_FORMAT_DIRECT);

	WaveInHdr.lpData = (LPSTR)data;
	WaveInHdr.dwBufferLength = length * 2;
	WaveInHdr.dwBytesRecorded=0;
	WaveInHdr.dwUser = 0L;
	WaveInHdr.dwFlags = 0L;
	WaveInHdr.dwLoops = 0L;
	waveInPrepareHeader(hWaveIn, &WaveInHdr, sizeof(WAVEHDR));

	waveInAddBuffer(hWaveIn, &WaveInHdr, sizeof(WAVEHDR));

	waveInStart(hWaveIn);
	while(waveInUnprepareHeader(hWaveIn,&WaveInHdr,sizeof(WAVEHDR)) == WAVERR_STILLPLAYING);
	waveInClose(hWaveIn);
}

/*
	Save recording in text file to predict given command
*/
void static SaveRecording(short int *data, int size, char *file)
{
    printf("Saving...\n");
	fprintf(fp_log, "Saving...\n");
    FILE *f=fopen(file,"w");
    for(int i=0; i<size; i++){
        fprintf(f, "%hi\n", data[i]);
    }
    fclose(f);
}

/*
	Record a command from user
*/
static void  recording(char *file, int duration)
{
    int size = (SAMPLERATE * duration);
	short int *data = (short int *)malloc(sizeof(short int)*size);
    StartRecord(data, size);
    SaveRecording(data, size, file);
}


// ---------------------------------- Universe creation starts ----------------------------------

static void  construct_data_file_path(char* directory, char* word, int utterance, char* filepath){
	/* This function constructs the path for the data file. */

	char no[10];
	strcpy(filepath, directory);
	strcat(filepath, "/");
	strcat(filepath, word);
	strcat(filepath, "/");
	strcat(filepath, word);
	strcat(filepath, "_");
	sprintf(no, "%d", utterance);
	strcat(filepath, no);
	strcat(filepath, ".txt");
}

static void  construct_model_file_path(char* word, int utterance, int iteration, char* filepath){
	/* This function constructs the path for the model file for an utterance of a digit. */

	char no[10];
	strcpy(filepath, "converged models for utterances/");
	strcat(filepath, word);
	strcat(filepath, "/");
	strcat(filepath, word);
	strcat(filepath, "_");
	sprintf(no, "%d", utterance);
	strcat(filepath, no);
	strcat(filepath, "_");
	sprintf(no, "%d", iteration);
	strcat(filepath, no);
	strcat(filepath, ".txt");
}

static void  construct_final_model_file_path(char* word, int iteration, char* filepath){
	/* This function constructs the path for the model file for a digit. */

	char no[10];
	strcpy(filepath, "converged models for words/");
	strcat(filepath, word);
	strcat(filepath, "/");
	strcat(filepath, word);
	strcat(filepath, "_");
	sprintf(no, "%d", iteration);
	strcat(filepath, no);
	strcat(filepath, ".txt");
}


static void  fetch_and_preprocess_data(FILE *ip_fptr, long double *samples, int *no_of_samples){
	/* This function fetches the data from the file and remove the dc_shift and background noise from the data and store it in an array */

		int count = 0;
	long double value, avg_dc_shift, max_value = 0, sum = 0;
	*no_of_samples = 0;
	// We'll take samples(= DC_SHIFT_THRESHOLD) from the start point to compute the DC shift and background noise.
	while((*no_of_samples) != DC_SHIFT_THRESHOLD && !feof(ip_fptr)){
		fscanf(ip_fptr, "%lf", &value);
		samples[(*no_of_samples)++] = value;
		sum += value;
	}
	avg_dc_shift = sum/DC_SHIFT_THRESHOLD;

	// Subtract the DC shift from all the samples and store it in an array of samples and calculate the absolute maximum energy value for normalization.
	for(int i = 0; i < DC_SHIFT_THRESHOLD; i++)
		samples[i] -= avg_dc_shift;

	// Getting rest of the samples from the file.
	while(!feof(ip_fptr) && *(no_of_samples)){
		fscanf(ip_fptr, "%lf", &value);
		value -= avg_dc_shift;
		samples[(*no_of_samples)++] = value;  // Store the sample values after subtracting the DC shift.
		if(value < 0)
			value = -value;
		if(value > max_value)
			max_value = value;
	}

	// Normalizing the sample values using factor = NORMALIZATION_FACTOR.
	for(int i = 0; i < *no_of_samples; i++){
		samples[i] = (samples[i] / max_value) * NORMALIZATION_FACTOR;
	}
}


static void  apply_hamming_window(long double frames[FRAME_SIZE]){
	/* This function applies hamming window to all the steady frames we have for a vowel. */

	for(int i = 0; i < FRAME_SIZE; i++){
		frames[i] *= (0.54 - 0.46 * cos((2 * (long double)22/7 * i) / (FRAME_SIZE - 1))); // Applying hamming window.
	}
}

static void  find_correaltion_values(long double frames[FRAME_SIZE], long double correlation[CEPSTRAL_COEFF_PER_FRAME+1]){
	/* This function finds correaltion values for all the steady frames. We will get a matrix of size NO_OF_STEADY_FRAMES * CEPSTRAL_COEFF_PER_FRAME for a vowel containing correlation values. */

	for(int k = 0; k <= CEPSTRAL_COEFF_PER_FRAME; k++){
		correlation[k] = 0;
		for(int m = 0; m <= (FRAME_SIZE - 1 - k); m++){
			correlation[k] += frames[m] * frames[m + k]; // Calculating correlation value.
		}
	}
}

static void  durbin_algo(long double correlation[CEPSTRAL_COEFF_PER_FRAME+1], long double alpha_durbin[CEPSTRAL_COEFF_PER_FRAME+1]){
	/* This function is an implementation of the durbin's algorithm to get alpha values using correltion values. */

	long double alpha_values[CEPSTRAL_COEFF_PER_FRAME+1][CEPSTRAL_COEFF_PER_FRAME+1];
	// Step-1 of durbin's algorithm: assign correlation value to E.
	long double k, sum = 0, E = correlation[0];
	for(int i = 1; i <= CEPSTRAL_COEFF_PER_FRAME; i++){
		// Step-2 of durbin's algorithm: calculating value of k.
		sum = 0;
		for(int j = 1; j < i; j++){
			sum += (alpha_values[j][i-1] * correlation[i-j]);
		}
		k = (correlation[i] - sum)/E;

		// Step-3 of durbin's algorithm: assigning value of k to alpha.
		alpha_values[i][i] = k;

		// Step-4 of durbin's algorithm: calculating values of alpha.
		for(int j = 1; j < i; j++){
			alpha_values[j][i] = alpha_values[j][i-1] - k * alpha_values[i-j][i-1];
		}

		// Step-5 of durbin's algorithm: calculating value of E.
		E = (1 - k*k) * E;
	}
	for(int j = 1; j <= CEPSTRAL_COEFF_PER_FRAME; j++){
		alpha_durbin[j] = alpha_values[j][CEPSTRAL_COEFF_PER_FRAME];
	}
}

static void  find_cepstral_coeffs_for_frame(long double alpha_durbin[CEPSTRAL_COEFF_PER_FRAME+1], long double cepstral_coeff[CEPSTRAL_COEFF_PER_FRAME+1]){
	/* This function computes cepstral coefficients from the alpha values we have for each of the steady frames for a vowel. */

	// Looping through all the steady frames for a vowel.
	for(int m = 1; m <= CEPSTRAL_COEFF_PER_FRAME; m++){
		long double sum = 0;
		for(int k = 1; k <= m-1; k++){
			sum += ((float)k/m) * cepstral_coeff[k] * alpha_durbin[m-k]; // Computing cepstral coefficient.
		}
		cepstral_coeff[m] = alpha_durbin[m] + sum;
	}
}

static void  apply_raised_sine_window(long double cepstral_coeff[CEPSTRAL_COEFF_PER_FRAME+1]){
	/* This function applies raised sine window to cepstral coefficients we have for each of the steady frames for a vowel. */

	for(int m = 1; m <= CEPSTRAL_COEFF_PER_FRAME; m++){
		cepstral_coeff[m] *= 1 + ((long double)CEPSTRAL_COEFF_PER_FRAME/2) * sin(((long double)22/7 * m)/CEPSTRAL_COEFF_PER_FRAME); // applying raised sine window.
	}
}


static void  calculate_cepstral_coefficients(long double *samples, int no_of_samples, FILE* universe_fptr){
	/* This function calculates the cepstral coefficients for the data. */

	long double frames[FRAME_SIZE], correlation[CEPSTRAL_COEFF_PER_FRAME+1], alpha_durbin[CEPSTRAL_COEFF_PER_FRAME+1], cepstral_coeff[CEPSTRAL_COEFF_PER_FRAME+1];

	// Here we are taking frames of size FRAME_SIZE and next frame starts from slide of SLIDE_LENGTH
	for(int i = 0; i < no_of_samples; i += SLIDE_LENGTH){
		if(i + FRAME_SIZE < no_of_samples){
			int k = 0;
			for(int j = i; j < i + FRAME_SIZE; j++){
				frames[k++] = samples[j];
			}

			// Step-1: Applying hamming window on all the steady frames individually.
			apply_hamming_window(frames);

			// Step-2: Finding correlation(Ri's) values for all the steady frames.
			find_correaltion_values(frames, correlation);

			// Step-3: Finding Ai's values for all the steady frames using Ri values.(Using Durbin's Algorithm)
			durbin_algo(correlation, alpha_durbin);

			// Step-4: Finding Ci values for all the steady frames using Ai values.
			find_cepstral_coeffs_for_frame(alpha_durbin, cepstral_coeff);

			// Step-5: Applying raised sine window to all the steady frames individually.
			apply_raised_sine_window(cepstral_coeff);

			for(int k = 1; k <= CEPSTRAL_COEFF_PER_FRAME; k++){
				fprintf(universe_fptr, "%lf ", cepstral_coeff[k]); // Storing cepstral coefficients of all the reference files for a vowel.
			}
			fprintf(universe_fptr, "%c\n", ' ');
		}
	}
}

static void  generate_universe_from_dataset(char* directory){
	/* This function generates universe from the dataset. */

	FILE *word_fptr, *universe_fptr;
	char filepath[100];
	long double samples[MAX_NO_OF_SAMPLES];
	int no_of_samples = 0;

	universe_fptr = fopen("universe.txt", "w");
	for(int i = 0; i < NO_OF_WORDS; i++){
		char* word = dictionary[i];
		for(int j = 1; j <= training_size_per_word; j++){
			construct_data_file_path(directory, word, j, filepath);
			word_fptr = fopen(filepath, "r");
			if(!word_fptr){
				fprintf(fp_log, "File: %s doesn't exists!!!\n", filepath);
				fclose(fp_log);
			}
			fetch_and_preprocess_data(word_fptr, samples, &no_of_samples);
			calculate_cepstral_coefficients(samples, no_of_samples, universe_fptr);
			fclose(word_fptr);
		}
	}
	fclose(universe_fptr);
}


// ---------------------------------- Universe creation ends ----------------------------------

// ---------------------------------- Codebook creation starts ----------------------------------


static void  initialize_codebook_from_universe(FILE* universe_fptr, int* universe_size, long double centroids[MAX_CODEBOOK_SIZE][CEPSTRAL_COEFF_PER_FRAME]){
	/* This function fetches the universe data from the input CSV file and initalizes the codebook. */

	char temp;
	long double data;

	// Initializing the codebook of size 1 initially.
	for(int i = 0; i < CEPSTRAL_COEFF_PER_FRAME; i++)
		centroids[0][i] = 0;

	while(!feof(universe_fptr)){
		int i;
		for(i = 0; i < CEPSTRAL_COEFF_PER_FRAME; i++){
			fscanf(universe_fptr, "%lf", &data);
			centroids[0][i] += data; // Taking sum of all the data points in the universe to compute the initial codebook.
		}
		(*universe_size)++; // maintaining the number of data points in the universe.
	}

	// Averaging out the data points to get the intitial codebook.
	for(int i = 0; i < CEPSTRAL_COEFF_PER_FRAME; i++)
		centroids[0][i] /= *universe_size;

	rewind(universe_fptr);
}


static void  increase_codebook_size(int* codebook_size, long double centroids[MAX_CODEBOOK_SIZE][CEPSTRAL_COEFF_PER_FRAME], long double epsilon){
	/* This function doubles the size of the codebook in every iteration of the LBG algorithm. */

	// To store the previous codebook to a different array so that we can update out original codebook array with new entries.
	long double prev_centroids[MAX_CODEBOOK_SIZE][CEPSTRAL_COEFF_PER_FRAME];
	for(int i = 0; i < *codebook_size; i++){
		for(int j = 0; j < CEPSTRAL_COEFF_PER_FRAME; j++)
			prev_centroids[i][j] = centroids[i][j];
	}

	// Computing factor using which we will double the size of codebook.
	long double epsilon_plus = 1 + epsilon, epsilon_minus = 1 - epsilon;
	for(int i = 0; i < *codebook_size; i++){
		for(int j = 0; j < CEPSTRAL_COEFF_PER_FRAME; j++){
			// Computing two code vectors from one code vector in the codebook using the factors.
			centroids[i][j] = prev_centroids[i][j] * epsilon_plus;
			centroids[i+*codebook_size][j] = prev_centroids[i][j] * epsilon_minus;
		}
	}
	*codebook_size *= 2; // Doubling the codebook size.
}


static  long double compute_tokhura_distance(long double x[CEPSTRAL_COEFF_PER_FRAME], long double y[CEPSTRAL_COEFF_PER_FRAME]){
	/* This function conmputes tokhurs's distance between a centroid vector and data point vector from the universe. */

	long double distance = 0, diff;
	for(int i = 0; i < CEPSTRAL_COEFF_PER_FRAME; i++){
		diff = x[i] - y[i];
		distance += tokhura_weights[i] * diff * diff; // Computing tokhura's distance.
	}
	return distance;
}


static  long double calculate_average_distortion(FILE* universe_fptr, int universe_size, int codebook_size, long double centroids[MAX_CODEBOOK_SIZE][CEPSTRAL_COEFF_PER_FRAME], int classified_universe[MAX_UNIVERSE_SIZE]){
	/* This function computes average distortion for the entire universe after the clusters are updated by computing new centroids. */

	long double sum_of_distances = 0;
	long double datapoint[CEPSTRAL_COEFF_PER_FRAME];
	char temp;

	// Distortion is computed using tokhura's distance between a data point and centroid of the cluster to which that data point belongs.
	for(int i = 0; i < universe_size; i++){
		for(int j = 0; j < CEPSTRAL_COEFF_PER_FRAME; j++){
			fscanf(universe_fptr, "%lf", &datapoint[j]);
		}

		int cluster_no = classified_universe[i];
		sum_of_distances += compute_tokhura_distance(datapoint, centroids[cluster_no]);
	}

	rewind(universe_fptr);

	// Averaging the distances to get the average distortion.
	return sum_of_distances/universe_size;
}


static void  find_nearest_neighbour(FILE* universe_fptr, int universe_size, int codebook_size, long double centroids[MAX_CODEBOOK_SIZE][CEPSTRAL_COEFF_PER_FRAME], int classified_universe[MAX_UNIVERSE_SIZE], int datapoints_per_cluster[MAX_CODEBOOK_SIZE]){
	/* This function computes the nearest centroid for each data points in the universe dataset. */

	// To maintain the number of data points that are mapped to particular cluster.
	for(int i = 0; i < codebook_size; i++)
		datapoints_per_cluster[i] = 0;

	long double datapoint[CEPSTRAL_COEFF_PER_FRAME];
	for(int i = 0; i < universe_size; i++){
		for(int j = 0; j < CEPSTRAL_COEFF_PER_FRAME; j++){
			fscanf(universe_fptr, "%lf", &datapoint[j]);
		}

		long double distance, min_distance = INT_MAX;
		int min_distance_index; // To store the cluster number to which a particular data point is mapped.
		for(int j = 0; j < codebook_size; j++){
			distance = compute_tokhura_distance(datapoint, centroids[j]); // Computing the distance between a data point and all the centroids to find the nearest neighbour.
			if(distance < min_distance){
				min_distance = distance;
				min_distance_index = j;
			}
		}
		classified_universe[i] = min_distance_index; // Maintaining the cluster to which each of the data points are mapped.
		datapoints_per_cluster[min_distance_index]++; // Maintaining the number of data points per cluster.
	}
	rewind(universe_fptr);
}


static void  update_centroids(FILE* universe_fptr, int universe_size, int codebook_size, long double centroids[MAX_CODEBOOK_SIZE][CEPSTRAL_COEFF_PER_FRAME], int classified_universe[MAX_UNIVERSE_SIZE], int datapoints_per_cluster[MAX_CODEBOOK_SIZE]){
	/* This function updates the centroid of all the clusters after the nearest neighbours are changed for each of the data points. */

	// Making the previos centroids to 0 because we will compute the new centroids for all the clusters.
	for(int i = 0; i < codebook_size; i++){
		for(int j = 0; j < CEPSTRAL_COEFF_PER_FRAME; j++)
			centroids[i][j] = 0;
	}

	// Summing up all the data points belonging to a particular cluster and doing this for all the clusters.
	long double datapoint[CEPSTRAL_COEFF_PER_FRAME];
	for(int i = 0; i < universe_size; i++){
		for(int j = 0; j < CEPSTRAL_COEFF_PER_FRAME; j++){
			fscanf(universe_fptr, "%lf", &datapoint[j]);
		}
		int cluster_no = classified_universe[i];
		for(int j = 0; j < CEPSTRAL_COEFF_PER_FRAME; j++)
			centroids[cluster_no][j] += datapoint[j];
	}

	// Computing average values for all the for all the clusters to get the new centroids.
	for(int i = 0; i < codebook_size; i++){
		for(int j = 0; j < CEPSTRAL_COEFF_PER_FRAME; j++)
			centroids[i][j] /= datapoints_per_cluster[i];
	}
	rewind(universe_fptr);
}


static void  run_kmeans_algorithm(FILE* universe_fptr, int universe_size, int codebook_size, long double centroids[MAX_CODEBOOK_SIZE][CEPSTRAL_COEFF_PER_FRAME], long double delta, int classified_universe[MAX_UNIVERSE_SIZE], FILE* fptr){
	/* This function is the execution of the K-means algorithm. */

	int datapoints_per_cluster[MAX_CODEBOOK_SIZE], iterations = 1;
	long double distortion, prev_distortion = INT_MAX;
	while(1){
		// Step-1: Computing the nearest neighbours for all the data points in the universe.
		find_nearest_neighbour(universe_fptr, universe_size, codebook_size, centroids, classified_universe, datapoints_per_cluster);

		// Step-2: Updating the centroids for all the cluster after we updated the clusters for all the data points.
		update_centroids(universe_fptr, universe_size, codebook_size, centroids, classified_universe, datapoints_per_cluster);

		// Step-3: Calculating average distortion for the universe dataset.
		distortion = calculate_average_distortion(universe_fptr, universe_size, codebook_size, centroids, classified_universe);

		// Step-4: If distortion value is not reduced by a threshold which is delta then stop the execution of K-means algorithm otherwise, go to the next iteration of K-means algorithm.
		if((prev_distortion - distortion) <= delta)
			break;
		prev_distortion = distortion;
		printf("- Iteration-%d of K-means algorithm, distortion = %lf\n", iterations, distortion);
		fprintf(fp_log, "- Iteration-%d of K-means algorithm, distortion = %lf\n", iterations, distortion);
		fprintf(fptr, "%lf\n", distortion); // Storing distortion values in the file.
		iterations++;
	}
}


static void  run_lbg_algorithm(FILE* universe_fptr, int universe_size, long double centroids[MAX_CODEBOOK_SIZE][CEPSTRAL_COEFF_PER_FRAME], long double delta, long double epsilon){
	/* This function is the execution of the LBG algorithm. */
	int codebook_size = 1, iterations = 1;
	int classified_universe[MAX_UNIVERSE_SIZE];
	long double distortion;
	
	// Initially when codebook size is one all the data points in the universe belongs to one cluster, so only one centroid.
	for(int i = 0; i < MAX_UNIVERSE_SIZE; i++)
		classified_universe[i] = 0;

	FILE* fptr = fopen("distortion_lbg.txt", "w"); // To store the distortion values we get per iteration of the LBG algorithm in a file.

	// We will execute the LBG algorithm until we get desired codebook size.
	while(codebook_size != MAX_CODEBOOK_SIZE){
		// Step-1: Doubling the codebook size.
		increase_codebook_size(&codebook_size, centroids, epsilon);

		// Calculating the average distortion after doubling the size of codebook.(So here the average distortion increases by a small value.)
		distortion = calculate_average_distortion(universe_fptr, universe_size, codebook_size, centroids, classified_universe);
		printf("\nIteration-%d of LBG algorithm with updated codebook size = %d and distortion = %lf\n", iterations, codebook_size, distortion);
		fprintf(fp_log, "\nIteration-%d of LBG algorithm with updated codebook size = %d and distortion = %lf\n", iterations, codebook_size, distortion);
		fprintf(fptr, "%lf\n", distortion); // Storing distortion values in the file.

		// Step-2: Run K-means algorithm with increased codebook size.
		run_kmeans_algorithm(universe_fptr, universe_size, codebook_size, centroids, delta, classified_universe, fptr);
		iterations++;
	}
	fclose(fptr);
}


static void  generate_codebook_from_universe(){
	/* This function generates codebook from the universe. */

	FILE* universe_fptr = fopen("universe.txt", "r");
	FILE* codebook_fptr = fopen("codebook.txt", "w");
	if(!universe_fptr){
		fprintf(fp_log, "File: %s doesn't exists!!!\n", "universe.txt");
		fclose(fp_log);
	}
	long double centroids[MAX_CODEBOOK_SIZE][CEPSTRAL_COEFF_PER_FRAME], delta = 0.0001, epsilon = 0.03;  // Here delta is initialized with value 0.0001 and epsilon is intialized with 0.03.
	int universe_size = 0, codebook_size = 1;

	initialize_codebook_from_universe(universe_fptr, &universe_size, centroids);
	run_lbg_algorithm(universe_fptr, universe_size, centroids, delta, epsilon);
	for(int i = 0; i < MAX_CODEBOOK_SIZE; i++){
		for(int j = 0; j < CEPSTRAL_COEFF_PER_FRAME; j++){
			fprintf(codebook_fptr, "%lf ", centroids[i][j]);
		}
		fprintf(codebook_fptr, "%c\n", ' ');
	}
	fclose(universe_fptr);
	fclose(codebook_fptr);
}

// ---------------------------------- Codebook creation ends ----------------------------------



// ---------------------------------- Model generation starts ---------------------------------


static  int get_codebook_index(long double cepstral_coeff[CEPSTRAL_COEFF_PER_FRAME+1], long double codebook[MAX_CODEBOOK_SIZE][CEPSTRAL_COEFF_PER_FRAME]){
	/* This functions computes codebook index for a frame from its cepstral coefficients. */

	long double min_distance = INT_MAX, distance;
	int min_distance_index = 0;

	long double coeff[CEPSTRAL_COEFF_PER_FRAME];
	for(int i = 0; i < CEPSTRAL_COEFF_PER_FRAME; i++)
		coeff[i] = cepstral_coeff[i+1];


	for(int i = 0; i < MAX_CODEBOOK_SIZE; i++){
		// Here it uses tokhura's distance to compute the closest codebook entry for the frame.
		distance = compute_tokhura_distance(coeff, codebook[i]);
		if(distance < min_distance){
			min_distance = distance;
			min_distance_index = i + 1;
		}
	}
	return min_distance_index;
}

static void  get_observation_sequence(long double *samples, int no_of_samples, long double codebook[MAX_CODEBOOK_SIZE][CEPSTRAL_COEFF_PER_FRAME]){
	long double frames[FRAME_SIZE], correlation[CEPSTRAL_COEFF_PER_FRAME+1], alpha_durbin[CEPSTRAL_COEFF_PER_FRAME+1], cepstral_coeff[CEPSTRAL_COEFF_PER_FRAME+1];
	observation_seq_len = 1;
	for(int i = 0; i < no_of_samples && observation_seq_len < T+1; i += SLIDE_LENGTH){
		if(i + FRAME_SIZE < no_of_samples){
			int k = 0;
			for(int j = i; j < i + FRAME_SIZE; j++){
				frames[k++] = samples[j];
			}

			// Step-1: Applying hamming window on all the steady frames individually.
			apply_hamming_window(frames);

			// Step-2: Finding correlation(Ri's) values for all the steady frames.
			find_correaltion_values(frames, correlation);

			// Step-3: Finding Ai's values for all the steady frames using Ri values.(Using Durbin's Algorithm)
			durbin_algo(correlation, alpha_durbin);

			// Step-4: Finding Ci values for all the steady frames using Ai values.
			find_cepstral_coeffs_for_frame(alpha_durbin, cepstral_coeff);

			// Step-5: Applying raised sine window to all the steady frames individually.
			apply_raised_sine_window(cepstral_coeff);

			// Step-6: Get observation sequence
			observation[(observation_seq_len)++] = get_codebook_index(cepstral_coeff, codebook);
		}
	}
	(observation_seq_len)--;
}

static void  initialize_model(){
	/* This function initialize the model parameters using interia model. */

	FILE* A_fptr = fopen("inertia_model_AIJ.txt", "r");
	if(!A_fptr){
		fprintf(fp_log, "File: %s doesn't exists!!!\n", "inertia_model_AIJ.txt");
		fclose(fp_log);
	}
	for(int i = 1; i <= N; i++){
		for(int j = 1; j <= N; j++){
			fscanf(A_fptr, "%lf", &A[i][j]);
		}
	}
	fclose(A_fptr);

	for(int i = 1; i <= N; i++){
		for(int j = 1; j <= M; j++){
			B[i][j] = 1.0/32.0;
		}
	}

	for(int i = 1; i <= N; i++)
		pi[i] = 0;
	pi[1] = 1;
}



static void  compute_alpha(){
	/* This function computes alpha values. */

	// Step-1: Initalization
	for(int i = 1; i <= N; i++)
		alpha[1][i] = pi[i] * B[i][observation[1]];

	// Step-2: Induction
	for(int t = 1; t <= observation_seq_len-1; t++){
		for(int j = 1; j <= N; j++){
			long double sum = 0;
			for(int i = 1; i <= N; i++)
				sum += alpha[t][i] * A[i][j];
			alpha[t+1][j] = sum * B[j][observation[t+1]];
		}
	}

	// Step-3: Termination 
	long double sum = 0;
	for(int i = 1; i <= N; i++)
		sum += alpha[observation_seq_len][i];

	prob_of_O_given_lambda = sum;
}

static void  compute_beta(){
	/* This function computes beta values. */

	// Step-1: Initalization
	for(int i = 1; i <= N; i++)
		beta[observation_seq_len][i] = 1;

	// Step-2: Induction
	for(int t = observation_seq_len-1; t >= 1; t--){
		for(int i = 1; i <= N; i++){
			long double sum = 0;
			for(int j = 1; j <= N; j++)
				sum += A[i][j] * B[j][observation[t+1]] * beta[t+1][j];
			beta[t][i] = sum;
		}
	}
}


static void  compute_gamma(){
	/* This function computing gamma matrix using alpha and beta matrices. */

	for(int t = 1; t <= observation_seq_len; t++){
		long double sum = 0;
		for(int i = 1; i <= N; i++)
			sum += alpha[t][i] * beta[t][i];

		for(int i = 1; i <= N; i++)
			gamma[t][i] = (alpha[t][i] * beta[t][i]) / sum;
	}
}


static void  get_state_sequence_from_gamma(){
	/* This function fetches the state sequence for a given observation sequence using gamma matrix.. */

	long double max;
	int argmax;
	FILE* state_fptr = fopen("state_sequence_using_gamma.txt", "w");

	// Storing observation sequence and corresponding state sequence in the file. 
	for(int i = 1; i <= observation_seq_len; i++)
		fprintf(state_fptr, "%4d ", observation[i]);
	fprintf(state_fptr, "%c", '\n');
	printf("\nState sequence using gamma matrix: ");
	for(int t = 1; t <= observation_seq_len; t++){
		max = 0;
		for(int i = 1; i <= N; i++){
			if(gamma[t][i] >= max){
				max = gamma[t][i];
				argmax = i;
			}
		}
		fprintf(state_fptr, "%4d ", argmax);
		printf("%d ", argmax);
	}
	fclose(state_fptr);
}

static void  viterbis_algo(){
	/* This function implements viterbi's algo. */

	// Step-1: Initialization
	for(int i = 1; i <= N; i++){
		delta[1][i] = pi[i] * B[i][observation[1]];
		psi[1][i] = 0;
	}

	// Step-2: Recursion
	long double val, max;
	int argmax;
	for(int t = 2; t <= observation_seq_len; t++){
		for(int j = 1; j <= N; j++){
			max = 0;
			argmax = 1;
			for(int i = 1; i <= N; i++){
				val = delta[t-1][i] * A[i][j];
				if(val > max){
					max = val;
					argmax = i;
				}
			}
			delta[t][j] = max * B[j][observation[t]];
			psi[t][j] = argmax;
		}
	}

	// Step-3: Termination
	max = 0;
	for(int i = 1; i <= N; i++){
		if(delta[observation_seq_len][i] > max){
			max = delta[observation_seq_len][i];
			argmax = i;
		}
	}
	p_star = max;
	q_star[observation_seq_len] = argmax;

	// printf("\n\nP*: %e\n", p_star);

	// Step-4: State sequence backtracking
	for(int t = observation_seq_len-1; t >= 1; t--)
		q_star[t] = psi[t+1][q_star[t+1]];
}

static void  reestimate_model_parameters(){
	/* This function reestimates model parameters. */

	// Computing Xi matrix.
	for(int t = 1; t <= observation_seq_len-1; t++){
		long double denominator = 0;
		for(int i = 1; i <= N; i++){
			for(int j = 1; j <= N; j++){
				xi[t][i][j] = alpha[t][i] * A[i][j] * B[j][observation[t+1]] * beta[t+1][j];
				denominator += alpha[t][i] * A[i][j] * B[j][observation[t+1]] * beta[t+1][j];
			}
		}
		for(int i = 1; i <= N; i++){
			for(int j = 1; j <= N; j++){
				xi[t][i][j] /= denominator;   // We can directly divide it with the prob_of_O_given_lambda we got while computing alpha matrix.
			}
		}
	}

	// No need to re-estimate pi vector.
	// // Re-estimating PI vector.
	// for(int i = 1; i <= N; i++)
	// 	pi[i] = gamma[1][i];

	// Re-estimating A matrix.
	for(int i = 1; i <= N; i++){
		for(int j = 1; j <= N; j++){
			long double xi_sum = 0, gamma_sum = 0;
			for(int t = 1; t <= observation_seq_len-1; t++){
				xi_sum += xi[t][i][j];
				gamma_sum += gamma[t][i];
			}
			A[i][j] = xi_sum / gamma_sum;
		}
	}

	// Re-estimating B matrix.
	for(int j = 1; j <= N; j++){
		for(int k = 1; k <= M; k++){
			long double numerator = 0, denominator = 0;
			for(int t = 1; t <= observation_seq_len; t++){
				if(observation[t] == k)
					numerator += gamma[t][j];
				denominator += gamma[t][j];
			}
			B[j][k] = numerator / denominator;
		}
	}
}

static void  make_updated_matrices_stochastic(){
	/* This function reevaluate the model paramters to make them stochastic after reestimation. */

	long double max_val = 0, sum = 0;
	int max_index = 1;
	for(int i = 1; i <= N; i++){
		sum = 0;
		max_val = 0;
		max_index = 1;
		for(int j = 1; j <= N; j++){	
			sum += A[i][j];
			if(A[i][j] > max_val){
				max_val = A[i][j];
				max_index = j;
			}
		}
		A[i][max_index] += (1.0 - sum);
	}


	long double small_val = 1e-30;
	for(int i = 1; i <= N; i++){
		sum = 0;
		max_val = 0;
		max_index = 1;
		for(int j = 1; j <= M; j++){	
			if(B[i][j] == 0){
				sum += small_val;
				B[i][j] = small_val;
			}
			if(B[i][j] > max_val){
				max_val = B[i][j];
				max_index = j;
			}
		}
		B[i][max_index] -= sum;
	}
}

static void  compute_model_for_digit(FILE* hmm_iter_fptr){
	/* This function computes converged models for all the digits in the training data. */

	long double p_star_old = INT_MAX;
	int no_of_iterations = 0;
	while(no_of_iterations < MAX_HMM_ITERATIONS){
		compute_alpha();
		compute_beta();
		compute_gamma();
		viterbis_algo();
		reestimate_model_parameters();
		make_updated_matrices_stochastic();
		if(p_star_old != INT_MAX && (p_star/p_star_old) <= 1)
			break;
		p_star_old = p_star;
		printf("\n- HMM Iteration-%d: P* = %e", no_of_iterations+1, p_star);
		fprintf(hmm_iter_fptr, "\n- HMM Iteration-%d: P* = %e", no_of_iterations+1, p_star);
		no_of_iterations++;
	}
}


static void  store_model_in_file(FILE* model_fptr){
	/* This function stores model parameters in the file. */

	for(int i = 1; i <= N; i++){
		for(int j = 1; j <= N; j++){
			fprintf(model_fptr, "%e ", A[i][j]);
		}
		fprintf(model_fptr, "\n");
	}
	for(int i = 1; i <= N; i++){
		for(int j = 1; j <= M; j++){
			fprintf(model_fptr, "%e ", B[i][j]);
		}
		fprintf(model_fptr, "\n");
	}
}


static void  initialize_sum_matrices(long double A_matrix_sum[N+1][N+1], long double B_matrix_sum[N+1][M+1]){
	/* This function initializes sum matrix to accumulate all the models. */

	for(int i = 1; i <= N; i++){
		for(int j = 1; j <= N; j++){
			A_matrix_sum[i][j] = 0;
		}
	}
	for(int i = 1; i <= N; i++){
		for(int j = 1; j <= M; j++){
			B_matrix_sum[i][j] = 0;
		}
	}
}


static void  add_utterance_model_to_digit_model(long double A_matrix_sum[N+1][N+1], long double B_matrix_sum[N+1][M+1]){
	/* This function accumulates all the models. */

	for(int i = 1; i <= N; i++){
		for(int j = 1; j <= N; j++){
			A_matrix_sum[i][j] += A[i][j];
		}
	}
	for(int i = 1; i <= N; i++){
		for(int j = 1; j <= M; j++){
			B_matrix_sum[i][j] += B[i][j];
		}
	}
}


static void  store_averaged_digit_model(FILE* final_model_fptr, long double A_matrix_sum[N+1][N+1], long double B_matrix_sum[N+1][M+1], long double A_matrix_avg[N+1][N+1], long double B_matrix_avg[N+1][M+1]){
	/* This function stores averaged converged models in files. */

	for(int i = 1; i <= N; i++){
		for(int j = 1; j <= N; j++){
			A_matrix_avg[i][j] = A_matrix_sum[i][j]/training_size_per_word;
			fprintf(final_model_fptr, "%e ", A_matrix_avg[i][j]);
		}
		fprintf(final_model_fptr, "\n");
	}
	for(int i = 1; i <= N; i++){
		for(int j = 1; j <= M; j++){
			B_matrix_avg[i][j] = B_matrix_sum[i][j]/training_size_per_word;
			fprintf(final_model_fptr, "%e ", B_matrix_avg[i][j]);
		}
		fprintf(final_model_fptr, "\n");
	}
}


static void  initialize_model_with_previous_averaged_model(long double A_matrix_avg[N+1][N+1], long double B_matrix_avg[N+1][M+1]){
	/* This function initializes model parameters using the averaged model. */

	for(int i = 1; i <= N; i++){
		for(int j = 1; j <= N; j++){
			A[i][j] = A_matrix_avg[i][j];
		}
	}
	for(int i = 1; i <= N; i++){
		for(int j = 1; j <= M; j++){
			B[i][j] = B_matrix_avg[i][j];
		}
	}
}


static void  gernerate_models_for_dataset(char* directory){
	/* This function generates model for the training dataset. */

	FILE *word_fptr, *model_fptr, *final_model_fptr, *hmm_iter_fptr;
	char filepath[100];
	long double samples[MAX_NO_OF_SAMPLES];
	int no_of_samples = 0;
	char* word;
	// char* directory = "dataset";

	hmm_iter_fptr = fopen("HMM_iterations.txt", "w");
	
	FILE* codebook_fptr = fopen("codebook.txt", "r");
	if(!codebook_fptr){
		fprintf(fp_log, "File: %s doesn't exists!!!\n", "codebook.txt");
		fclose(fp_log);
	}
	long double codebook[MAX_CODEBOOK_SIZE][CEPSTRAL_COEFF_PER_FRAME];
	for(int i = 0; i < MAX_CODEBOOK_SIZE; i++){
		for(int j = 0; j < CEPSTRAL_COEFF_PER_FRAME; j++){
			fscanf(codebook_fptr, "%lf ", &codebook[i][j]);
		}
	}
	fclose(codebook_fptr);

	long double A_matrix_sum[N+1][N+1], B_matrix_sum[N+1][M+1], A_matrix_avg[N+1][N+1], B_matrix_avg[N+1][M+1];
	for(int i = 0; i < NO_OF_WORDS; i++){
		word = dictionary[i];
		for(int t = 1; t <= 2; t++){
			initialize_sum_matrices(A_matrix_sum, B_matrix_sum);
			for(int j = 1; j <= training_size_per_word; j++){
				printf("\n iteration = %d, word = %s, utterance = %d", t, word, j);
				fprintf(hmm_iter_fptr, "\n iteration = %d, word = %s, utterance = %d", t, word, j);
				construct_data_file_path(directory, word, j, filepath);
				word_fptr = fopen(filepath, "r");
				if(!word_fptr){
					fprintf(fp_log, "File: %s doesn't exists!!!\n", filepath);
					fclose(fp_log);
				}
				fetch_and_preprocess_data(word_fptr, samples, &no_of_samples);
				get_observation_sequence(samples, no_of_samples, codebook);

				if(t == 1)
					initialize_model();
				else
					initialize_model_with_previous_averaged_model(A_matrix_avg, B_matrix_avg);

				compute_model_for_digit(hmm_iter_fptr);
				construct_model_file_path(word, j, t, filepath);
				model_fptr = fopen(filepath, "w");
				store_model_in_file(model_fptr);
				add_utterance_model_to_digit_model(A_matrix_sum, B_matrix_sum);
				fclose(word_fptr);
				fclose(model_fptr);
			}
			construct_final_model_file_path(word, t, filepath);
			final_model_fptr = fopen(filepath, "w");
			store_averaged_digit_model(final_model_fptr, A_matrix_sum, B_matrix_sum, A_matrix_avg, B_matrix_avg);
			fclose(final_model_fptr);
		}
	}
	fclose(hmm_iter_fptr);
}

// ---------------------------------- Model generation ends -----------------------------------

// ---------------------------------- Testing module starts -----------------------------------

static void  get_model_from_file(FILE* final_model_fptr){
	/* This function fetches model from the file. */

	for(int i = 1; i <= N; i++){
		for(int j = 1; j <= N; j++){
			fscanf(final_model_fptr, "%lf", &A[i][j]);
		}
	}
	
	for(int i = 1; i <= N; i++){
		for(int j = 1; j <= M; j++){
			fscanf(final_model_fptr, "%lf", &B[i][j]);
		}
	}
}


static void  initalize_pi_values(){
	/* This function initalizes pi vector.*/

	for(int i = 1; i <= N; i++)
		pi[i] = 0;
	pi[1] = 1;
}

static void  predict_for_test_data(){
	/* This function predicts the digit for the test dataset. */

	FILE *final_model_fptr, *word_fptr, *codebook_fptr;
	char filepath[100];
	char* word;
	long double codebook[MAX_CODEBOOK_SIZE][CEPSTRAL_COEFF_PER_FRAME];
	char* directory = "dataset";
	

	codebook_fptr = fopen("codebook.txt", "r");
	if(!codebook_fptr){
		fprintf(fp_log, "File: %s doesn't exists!!!\n", "codebook.txt");
		fclose(fp_log);
	}
	for(int i = 0; i < MAX_CODEBOOK_SIZE; i++){
		for(int j = 0; j < CEPSTRAL_COEFF_PER_FRAME; j++){
			fscanf(codebook_fptr, "%lf ", &codebook[i][j]);
		}
	}
	fclose(codebook_fptr);
	initalize_pi_values();

	long double correct_prediction = 0;

	for(int i = 0; i < NO_OF_WORDS; i++){
		long double max_prob;
		word = dictionary[i];
		for(int j = TESTING_FILE_START_NO; j <= TESTING_FILE_END_NO; j++){
			printf("\noriginal word: %s, utterance: %d", word, j);
			fprintf(fp_log, "\noriginal word: %s, utterance: %d", word, j);
			int no_of_samples = 0;
			long double samples[MAX_NO_OF_SAMPLES];
			construct_data_file_path(directory, word, j, filepath);
			word_fptr = fopen(filepath, "r");
			if(!word_fptr){
				fprintf(fp_log, "File: %s doesn't exists!!!\n", filepath);
				fclose(fp_log);
			}
			fetch_and_preprocess_data(word_fptr, samples, &no_of_samples);
			get_observation_sequence(samples, no_of_samples, codebook);
			long double max_prob = 0;
			char* predicted_word = dictionary[0];
			for(int k = 0; k < NO_OF_WORDS; k++){
				construct_final_model_file_path(dictionary[k], 2, filepath);
				final_model_fptr = fopen(filepath, "r");
				if(!final_model_fptr){
					fprintf(fp_log, "File: %s doesn't exists!!!\n", filepath);
					fclose(fp_log);
				}
				get_model_from_file(final_model_fptr);
				compute_alpha();
			
				if(prob_of_O_given_lambda > max_prob){
					max_prob = prob_of_O_given_lambda;
					predicted_word = dictionary[k];
				}
				fclose(final_model_fptr);
			}
			printf(" => p(O/lambda): %e, predicted word: %s", max_prob, predicted_word);
			fprintf(fp_log, " => p(O/lambda): %e, predicted word: %s", max_prob, predicted_word);
			if(predicted_word == word)
				correct_prediction++;
			fclose(word_fptr);
		}
	}
	printf("\n\nModel Accuracy: %.2lf", correct_prediction/TOTAL_TESTING_SIZE * 100);
	fprintf(fp_log, "\n\nModel Accuracy: %.2lf", correct_prediction/TOTAL_TESTING_SIZE * 100);

}

//Preprocess Recorded file
static void preprocess_recorded_file(char file[200]){
	//Initialioze energy array to zero
	for(int i = 0; i < 2000; i++){
		energy[i] = 0;
	}
	//open inputfile
	FILE* recorded_fp=fopen(file,"r");
	if(recorded_fp==NULL){
		printf("Error in Opening File %s!\n",file);
		fprintf(fp_log, "File: %s doesn't exists!!!\n", file);
		fclose(fp_log);
		return;
	}
	int amp;
	int i=0;
	int j=0;
	while(!feof(recorded_fp)){
		fscanf(recorded_fp,"%d",&amp);
		energy[i]+=amp*amp;
		j++;
		if(j==80){
			i++;
			j=0;
		}
	}
	no_of_frames=i-3;
	fclose(recorded_fp);
}

//Normalize File
static void normalize_file(char fileName[200]){
	char output_filename[200] = "normalized_";
	strcat(output_filename,fileName);
	
	//open inputfile
	FILE* fp=fopen(fileName,"r");
	if(fp==NULL){
		printf("Error in Opening File %s!\n",fileName);
		fprintf(fp_log, "File: %s doesn't exists!!!\n", fileName);
		fclose(fp_log);
		return;
	}
	//open normalized file
	FILE* nfp=fopen(output_filename,"w");
	if(nfp==NULL){
		printf("Error in Opening File %s!\n",output_filename);
		return;
	}

	long double value, avg_dc_shift, max_value = 0, sum = 0, normalized_value;
	int count=0;
	long double samples[MAX_NO_OF_SAMPLES];
	int no_of_samples = 0;
	// We'll take samples(= DC_SHIFT_THRESHOLD) from the start point to compute the DC shift and background noise.
	while(count!=DC_SHIFT_THRESHOLD && !feof(fp)){
		fscanf(fp,"%Lf",&value);
		samples[(no_of_samples)++] = value;
		sum+=value;
		count++;
	}
	avg_dc_shift = sum/DC_SHIFT_THRESHOLD;

	// Subtract the DC shift from all the samples and store it in an array of samples and calculate the absolute maximum value for normalization.
	for(int i = 0; i < DC_SHIFT_THRESHOLD; i++)
		samples[i] -= avg_dc_shift;

	// Getting rest of the samples from the file.
	while(!feof(fp) && (no_of_samples)<MAX_NO_OF_SAMPLES){
		fscanf(fp, "%Lf", &value);
		value -= avg_dc_shift;
		samples[(no_of_samples)++] = value;  // Store the sample values after subtracting the DC shift.
		if(value < 0)
			value = -value;
		if(value > max_value)
			max_value = value;
	}

	// Normalizing the sample values using factor = NORMALIZATION_FACTOR.
	for(int i = 0; i < no_of_samples; i++){
		samples[i] = (samples[i] / max_value) * NORMALIZATION_FACTOR;
		fprintf(nfp,"%Lf\n",samples[i]);
	}

	fclose(nfp);
	fclose(fp);
}

//calculate energy of frame
static void  calculate_energy_of_frame(){
	int nf=no_of_frames;
	for(int f=0;f<nf;f++){
		energy[f]=(energy[f]+energy[f+1]+energy[f+2]+energy[f+3])/FS;
	}
}

//Calculate Max Energy of file
static  long double calculate_max_energy(){
	int nf=no_of_frames;
	long double max_energy=DBL_MIN;
	for(int f=0;f<nf;f++){
		if(energy[f]>max_energy){
			max_energy=energy[f];
		}
	}
	return max_energy;
}

//calculate average energy of file
static  long double calculate_avg_energy(){
	int nf=no_of_frames;
	long double avg_energy=0.0;
	calculate_energy_of_frame();
	for(int f=0;f<nf;f++){
		avg_energy+=(energy[f]/nf);
	}
	return avg_energy;
}

static  void initialize_fp_log(){
	fp_log = fopen("log_debug_null.txt", "w");
}

//mark starting and ending of speech activity
static  int mark_checkpoints(char file[200]){
	preprocess_recorded_file(file);
	
	//Make 10% of average energy as threshold
	long double threshold_energy=calculate_avg_energy() * 0.1;
	
	int nf=no_of_frames;
	int file_read_index=0;
	
	FILE *read_fp=fopen(file,"r");
	if(read_fp==NULL){
		printf("\nmark_checkpoints(): can't open file in read mode\n");
		fprintf(fp_log, "File: %s doesn't exists!!!\n", file);
		fclose(fp_log);
		return -1;
	}
	if(fp_log==NULL){
		printf("\nInside mark_checkpoints(): fp_log is null\n");
		initialize_fp_log();
	}
	
	printf("Threshold energy = %Lf\n", threshold_energy);
	fprintf(fp_log, "Threshold energy = %Lf\n", threshold_energy);
	
	start_frame=0;
	end_frame=0;
	int word=1;
	int f=0;
	
	while(f<nf-END_CHECKPOINT-10){
		int isAboveThresholdStart=1;
		int isAboveThresholdEnd=1;
		//Find start frame where speech activity starts
		for(f=end_frame+5;f<nf-START_CONSECUTIVE_FRAMES;f++){
			for(int i=0;i<START_CONSECUTIVE_FRAMES;i++){
				isAboveThresholdStart*=(energy[f+i]>threshold_energy);
			}
			if(isAboveThresholdStart){
				start_frame=((f-START_CHECKPOINT) > end_frame)?(f-START_CHECKPOINT):(end_frame);
				break;
			}
			isAboveThresholdStart=1;
		}
		if(f>nf-END_CHECKPOINT-10) break;
		//Find end frame where speech activity ends
		//printf("Start-Frame=%d\n",start_frame);		
		for(f=start_frame+5;f<nf-END_CONSECUTIVE_FRAMES;f++){
			for(int i=0;i<END_CONSECUTIVE_FRAMES;i++){
				isAboveThresholdEnd*=(energy[f+i]<threshold_energy);
			}
			if(isAboveThresholdEnd){
				end_frame=((f+END_CHECKPOINT) < nf)?(f+END_CHECKPOINT):(nf-1);
				break;
			}
			isAboveThresholdEnd=1;
		}
		if(f>nf-END_CHECKPOINT-10) break;
		int start_index=start_frame*80;
		int end_index=end_frame*80;
		
		//fprintf(fp_log, "Start-Frame=%d\n",start_frame);
		fprintf(fp_log, "Start-Index=%d\n",start_index);
		//fprintf(fp_log, "End-Frame=%d\n",end_frame);		
		fprintf(fp_log, "End-Index=%d\n",end_index);		
		
		int temp;
		while(file_read_index<start_index){
			fscanf(read_fp,"%d",&temp);
			file_read_index++;
		}
		
		char cut_file[100];
		sprintf(cut_file,"word_%d.txt",word);
		FILE* fp=fopen(cut_file,"w");
		for(int i=start_index;i<end_index;i++){
			fscanf(read_fp,"%d",&temp);
			file_read_index++;
			if(end_index-start_index<MAX_NO_OF_SAMPLES)
				fprintf(fp,"%d\n",temp);
		}
		fclose(fp);
		word++;
	}
	fclose(read_fp);
	return --word;
}

/*
	Predict one word for given file
*/
static  int predict_word(char *file){
	FILE *codebook_fptr, *file_fptr, *final_model_fptr;
	long double codebook[MAX_CODEBOOK_SIZE][CEPSTRAL_COEFF_PER_FRAME], samples[MAX_NO_OF_SAMPLES];
	int no_of_samples = 0;

	codebook_fptr = fopen("codebook.txt", "r");
	if(!codebook_fptr){
		fprintf(fp_log, "File: %s doesn't exists!!!\n", "codebook.txt");
		fclose(fp_log);
	}
	for(int i = 0; i < MAX_CODEBOOK_SIZE; i++){
		for(int j = 0; j < CEPSTRAL_COEFF_PER_FRAME; j++){
			fscanf(codebook_fptr, "%lf ", &codebook[i][j]);
		}
	}
	fclose(codebook_fptr);

	file_fptr = fopen(file, "r");
	if(!file_fptr){
		fprintf(fp_log, "File: %s doesn't exists!!!\n", file);
		fclose(fp_log);
	}
	fetch_and_preprocess_data(file_fptr, samples, &no_of_samples);
	get_observation_sequence(samples, no_of_samples, codebook);
	initalize_pi_values();

	long double max_prob = 0;
	int predicted_word_index = 3;
	char filepath[100];
	for(int k = 0; k < NO_OF_WORDS; k++){
		construct_final_model_file_path(dictionary[k], 2, filepath);
		final_model_fptr = fopen(filepath, "r");
		if(!final_model_fptr){
			fprintf(fp_log, "File: %s doesn't exists!!!\n", filepath);
			fclose(fp_log);
		}
		get_model_from_file(final_model_fptr);
		compute_alpha();
		fprintf(fp_log, "Probability with %s is %e\n", dictionary[k], prob_of_O_given_lambda);
		if(prob_of_O_given_lambda > max_prob){
			max_prob = prob_of_O_given_lambda;
			predicted_word_index = k;
		}
		fclose(final_model_fptr);
	}
	return predicted_word_index;
}

/*
	Record a command from user, cut the command into multiply words, predict that words and apply the appropriate action for particular command.
*/
static void  main_functionality(int index){
	char file[100] = "data", file_num[3];
	sprintf(file_num, "%d", index);
	strcat(file, file_num);
	strcat(file, ".txt");
	char normalized_file[100] = "normalized_";
	int predicted_word_index = 0;
	
	recording(file, COMMAND_DURATION);

	words = mark_checkpoints(file);
	for(int i=1;i<=words;i++){
		char word[100] = "word_", file_num[3];
		sprintf(file_num, "%d", i);
		strcat(word, file_num);
		strcat(word, ".txt");
		normalize_file(word);
	}
	printf("Number of words are %d\n", words);
	fprintf(fp_log, "Number of words are %d\n", words);
	predicted_words = (int*)malloc(words * sizeof(int));

	for(int i=1; i<=words; i++){
		char word[100] = "normalized_word_", file_num[3];
		sprintf(file_num, "%d", i);
		strcat(word, file_num);
		strcat(word, ".txt");
		printf("word file path is %s\n", word);
		fprintf(fp_log, "word file path is %s\n", word);
		predicted_word_index = predict_word(word);
		printf("Predicted word is %s\n", dictionary[predicted_word_index]);
		fprintf(fp_log, "Predicted word is %s\n", dictionary[predicted_word_index]);
		predicted_words[i-1] = predicted_word_index;
	}

	if(words > 0 && predicted_words[0] == 3){					// Open
		if(words > 1 && predicted_words[1] == 2){				// Open movies
			ShellExecuteA(NULL, "open", "movies", NULL, NULL, SW_MAXIMIZE);
		}
	} else if(words > 0 && predicted_words[0] == 5){			// VLC
		if(words > 1 && predicted_words[1] == 0){				// VLC close
			//system("TASKKILL /IM vlc.exe");
			ShellExecuteA(NULL, "open", "vbs_scripts\\close.vbs", NULL, NULL, SW_HIDE);
		} else if(words > 1 && predicted_words[1] == 6){		// VLC volume
			if(words > 2 && predicted_words[2] == 4){			// VLC volume up
				ShellExecuteA(NULL, "open", "vbs_scripts\\volume_up.vbs", NULL, NULL, SW_HIDE);
			} else if(words > 2 && predicted_words[2] == 1){	// VLC volume down
				ShellExecuteA(NULL, "open", "vbs_scripts\\volume_down.vbs", NULL, NULL, SW_HIDE);
			}
		}
	} else if(words > 0 && predicted_words[0] == 7){			// Window
		if(words > 1 && predicted_words[1] == 3){				// Window open
			ShellExecuteA(NULL, "open", "vbs_scripts\\enter.vbs", NULL, NULL, SW_HIDE);
		} else if(words > 1 && predicted_words[1] == 0){		// Window close
			ShellExecuteA(NULL, "open", "vbs_scripts\\close.vbs", NULL, NULL, SW_HIDE);
		} else if(words > 1 && predicted_words[1] == 4){		// Window up
			ShellExecuteA(NULL, "open", "vbs_scripts\\up.vbs", NULL, NULL, SW_HIDE);
		} else if(words > 1 && predicted_words[1] == 1){		// Window down
			ShellExecuteA(NULL, "open", "vbs_scripts\\down.vbs", NULL, NULL, SW_HIDE);
		}
	}
}

// ---------------------------------- Trainable module starts -----------------------------------


static void  copy_file(char* source_file, char* destination_file){
	long double data;
	FILE* source_fptr = fopen(source_file, "r");
	FILE* destination_fptr = fopen(destination_file, "w");
	if(!source_fptr){
		fprintf(fp_log, "File: %s doesn't exists!!!\n", source_file);
		fclose(fp_log);
	}
	while(!feof(source_fptr)){
		fscanf(source_fptr, "%lf", &data);
		fprintf(destination_fptr, "%lf\n", data);
	}
	fclose(source_fptr);
	fclose(destination_fptr);
}

static void  create_directory_structure(char* username){
	char directory[200] = "mkdir ", sub_directory[300];
	strcat(directory, trainable_module_directory);
	strcat(directory, "\\");
	strcat(directory, username);
	system(directory);

	strcat(directory, "\\");
	for(int i = 0; i < NO_OF_WORDS; i++){
		strcpy(sub_directory, directory);
		strcat(sub_directory, dictionary[i]);
		system(sub_directory);
	}
}


static void  record_data(char* username, char* word, int utterance){
	char directory[100], filepath[200];
	strcpy(directory, trainable_module_directory);
	strcat(directory, "/");
	strcat(directory, username);
	recording(recorded_file, WORD_DURATION);
	mark_checkpoints(recorded_file);
	normalize_file(trimmed_file);
	construct_data_file_path(directory, word, utterance, filepath);
	copy_file(normalized_trimmed_file, filepath);
}


static void  train_on_dataset(char* username, int total_utterances){
	char directory[100];
	strcpy(directory, trainable_module_directory);
	strcat(directory, "/");
	strcat(directory, username);
	// fprintf(fp_log, "%s, %s, %d", directory, username, total_utterances);
	training_size_per_word = total_utterances;
	generate_universe_from_dataset(directory);
	generate_codebook_from_universe();
	gernerate_models_for_dataset(directory);
}

// ---------------------------------- Trainable module ends -----------------------------------