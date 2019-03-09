#include <stdio.h>
#include <string.h>

float get_cpu_info ()
{ 
	FILE* fp; 
	char buffer[1024]; 
	size_t bytes_read; 
	char* match; 
	float clock_speed; 
	fp = fopen ("/proc/cpuinfo", "r"); 
	bytes_read = fread (buffer, 1, sizeof (buffer), fp); 
	fclose (fp);
	if (bytes_read == 0 || bytes_read == sizeof (buffer))  return 0; 
	/* NUL-terminate the text. */ 
	buffer[bytes_read] = ’\0’; 
	/* Locate the line that starts with "cpu MHz". */
	match = strstr (buffer, "cpu MHz"); 
	if (match == NULL)  return 0; 
	/* Parse the line to extract the clock speed. */ 
	sscanf (match, "cpu MHz : %f", &clock_speed);
	return clock_speed;
}

char* get_cpu_model ()
{ 
	FILE* fp; 
	char buffer[1024]; 
	size_t bytes_read; 
	char* match; 
	char* model_name; 
	fp = fopen ("/proc/cpuinfo", "r"); 
	bytes_read = fread (buffer, 1, sizeof (buffer), fp); 
	fclose (fp);
	if (bytes_read == 0 || bytes_read == sizeof (buffer))  return 0; 
	/* NUL-terminate the text. */ 
	buffer[bytes_read] = ’\0’; 
	/* Locate the line that starts with "cpu MHz". */
	match = strstr (buffer, "model name"); 
	if (match == NULL)  return 0; 
	/* Parse the line to extract the clock speed. */ 
	sscanf (match, "cpu MHz : %s", &model_name);
	return model_name;
}

int main (){ 
	printf ("CPU clock speed: %4.0f MHz\n", get_cpu_info ()); 
	return 0
}