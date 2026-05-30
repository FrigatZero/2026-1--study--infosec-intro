#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <stdbool.h>

#define MAX_BUFFER_SIZE 40
typedef unsigned char byte;



void char_2_hex(const char c, byte *val)
{
	if ((c >= '0') && (c <= '9'))
		*val = (byte)(c - '0');
	else if ((c >= 'a') && (c <= 'f'))
		*val = (byte)(c - 'a') + 10;
	else if ((c >= 'A') && (c <= 'F'))
		*val = (byte)(c - 'A') + 10;
}

void read_bytes(byte *buffer, char *data) 
{	
	int i = 0, j = 0;
	while (data[i] != '\0')
	{
		byte left = 0;
		byte right = 0;


		char_2_hex(data[i], &left);
		char_2_hex(data[i + 1], &right);

		
		buffer[j] = (left << 4) | (right & 0x0f);
		if (data[i + 2] == '\0') break;
		i += 3; j++;
	}
}

void print_as_bytes(byte *buffer)
{
	for (int i = 0; i < MAX_BUFFER_SIZE; i++)
	{
		printf("%02X ", buffer[i]);
	}
	printf("\n");
}
void print_as_chars(byte *buffer)
{
	for (int i = 0; i < MAX_BUFFER_SIZE; i++)
	{
		printf("%c", buffer[i]);
	}
	printf("\n");
}

void xor(byte *buffer, byte *open, byte *key) {
	for (int i = 0; i < MAX_BUFFER_SIZE; i++)
	{
		buffer[i] = open[i] ^ key[i];
	}
}

int main(int argc, char **argv) 
{
	byte       key[MAX_BUFFER_SIZE] = { 0 };
	byte      open[MAX_BUFFER_SIZE] = { 0 };
	byte encrypted[MAX_BUFFER_SIZE] = { 0 };

	bool hasKey = false;
	bool hasOpen = false;
	bool hasEncrypted = false;

	int i = 1;
	while (i < argc)
	{
		if (strcmp(argv[i], "-k") == 0) {
			if (i + 1 < argc) 
			{
				read_bytes(key, argv[i + 1]);
				hasKey = true;
			}
		}
		else if (strcmp(argv[i], "-o") == 0) {
			if (i + 1 < argc)
			{
				read_bytes(open, argv[i + 1]);
				hasOpen = true;
			}
		}
		else if (strcmp(argv[i], "-e") == 0) {
			if (i + 1 < argc)
			{
				read_bytes(encrypted, argv[i + 1]);
				hasEncrypted = true;
			}
		}
		i++;
	}

	printf("key:      (byte)"); print_as_bytes(key); printf("\n");
	printf("open:     (byte)"); print_as_bytes(open); printf("\n");
	printf("          (char)"); print_as_chars(open); printf("\n");
	printf("ecrypted: (byte)"); print_as_bytes(encrypted); printf("\n");

	byte buffer[MAX_BUFFER_SIZE] = { 0 };
	if (hasOpen && hasKey) {
		xor(buffer, open, key);
		printf("new_text:     (byte)"); print_as_bytes(buffer); printf("\n");
		xor(buffer, buffer, key);
		printf("test_decrypt: (byte)"); print_as_bytes(buffer); printf("\n");
		printf("              (char)"); print_as_chars(buffer); printf("\n");
	}
	if (hasEncrypted && hasKey) {
		xor(buffer, encrypted, key);
		printf("decrypt:      (byte)"); print_as_bytes(buffer); printf("\n");
		printf("              (char)"); print_as_chars(buffer); printf("\n");
	}
	if (hasEncrypted && hasOpen) {
		xor(buffer, encrypted, open);
		printf("desired_key:  (byte)"); print_as_bytes(buffer); printf("\n");
	}
}