/** @todo (Itsuki Fujii#1#): Need to be fixed:
*fixed*1.加密结果为单行时数据过长的截断问题。
*fixed*2.重复进行加解密时发生的错误。
*fixed*3.fgets()读取密文中换行后第一个字符时发生错误->该字符会被错误地读取为ASCII = 1
*fixed*4.单行加密时密文出现换行符的问题*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <windows.h>
#include <io.h>
#include <memory>
#include "process_bar.h"

using namespace std;

#define _CRT_SECURE_NO_WARNINGS 1
#define key_length 5
#define mod 98
#define mod_single_line 97
#define mx 1000000

int i, k, l, bytes = 1, current_key = 0, line = 1, cnvrtd_clr_text[mx], cnvrtd_keys[key_length], ciphertext_num[mx], cnvrtd_ciphertext[mx], clr_text_num[mx];
bool is_n = false, is_single_line = false;
char alphabet[mx] = "\x01\x09\x0a\x20\x21\x22\x23\x24\x25\x26\x27\x28\x29\x2a\x2b\x2c\x2d\x2e\x2f\x30\x31\x32\x33\x34\x35\x36\x37\x38\x39\x3a\x3b\x3c\x3d\x3e\x3f\x40\x41\x42\x43\x44\x45\x46\x47\x48\x49\x4a\x4b\x4c\x4d\x4e\x4f\x50\x51\x52\x53\x54\x55\x56\x57\x58\x59\x5a\x5b\x5c\x5d\x5e\x5f\x60\x61\x62\x63\x64\x65\x66\x67\x68\x69\x6a\x6b\x6c\x6d\x6e\x6f\x70\x71\x72\x73\x74\x75\x76\x77\x78\x79\x7a\x7b\x7c\x7d\x7e", alphabet_single_line[mx] = "\x01\x09\x20\x21\x22\x23\x24\x25\x26\x27\x28\x29\x2a\x2b\x2c\x2d\x2e\x2f\x30\x31\x32\x33\x34\x35\x36\x37\x38\x39\x3a\x3b\x3c\x3d\x3e\x3f\x40\x41\x42\x43\x44\x45\x46\x47\x48\x49\x4a\x4b\x4c\x4d\x4e\x4f\x50\x51\x52\x53\x54\x55\x56\x57\x58\x59\x5a\x5b\x5c\x5d\x5e\x5f\x60\x61\x62\x63\x64\x65\x66\x67\x68\x69\x6a\x6b\x6c\x6d\x6e\x6f\x70\x71\x72\x73\x74\x75\x76\x77\x78\x79\x7a\x7b\x7c\x7d\x7e", keys[key_length + 1], clr_text[mx], ciphertext[mx], c;

void sub()
{
	printf("\nRebooting");
	Sleep(400);
	printf(".");
	Sleep(400);
	printf(".");
	Sleep(400);
	printf(".");
	if (_access("cipher_sub.exe", 0) == 0)
	{
		system("cipher_sub.exe");
	}
	else
	{
		printf("Please check if the cipher_sub.exe is under the current path!\n");
		system("pause");
		exit(1);
	}
}

int chrctr_num(char c)
{
	//printf("chrctr_num->is_single_line = %s\n", is_single_line ? "true" : "false");
	if (!is_single_line)
	{
		for (i = 0; i < mod; i++)
		{
			if ((int)alphabet[i] == (int)c)
			{
				//printf("\nASCII No. = %d\n", (int) c);
				return i;
			}
		}
		printf("\nIllegal character detected! Ascii No.= %d, line = %d, position = %d\n", (int)c, line, bytes);
		system("pause");
		sub();
		return -99;
	}
	else
	{
		for (i = 0; i < mod_single_line; i++)
		{
			if ((int)alphabet_single_line[i] == (int)c)
			{
				//printf("\nASCII No. = %d\n", (int) c);
				return i;
			}
		}
		printf("\nIllegal character detected! Ascii No.= %d, line = %d, position = %d\n", (int)c, line, bytes);
		system("pause");
		sub();
		return -99;
	}
}

//void check(char c)
//{
//    if ((int) c == 1)
//        {
//            printf("got 1!\n");
//            system("pause");
//        }
//    else
//        {
//            printf("not 1!\n");
//        }
//}

void num_chrctr(int num)
{
	//printf("num_chrctr->is_single_line = %s\n", is_single_line ? "true" : "false");
	if (!is_single_line)
	{
		if (num > mod)
		{
			printf("Input error!\n");
			exit(1);
		}
		if (num < 0)
		{
			num += mod;
		}
		ciphertext[k] = alphabet[num];
	}
	else
	{
		if (num > mod_single_line)
		{
			printf("Input error!\n");
			exit(1);
		}
		if (num < 0)
		{
			num += mod_single_line;
		}
		ciphertext[k] = alphabet_single_line[num];
	}
	//check(ciphertext [k]);
}

void num_chrctr_2(int num)
{
	//printf("num_chrctr_2->is_single_line = %s\n", is_single_line ? "true" : "false");
	if (!is_single_line)
	{
		if (num > mod)
		{
			printf("Input error!\n");
			exit(1);
		}
		if (num < 0)
		{
			num += mod;
		}
		clr_text[k] = alphabet[num];
	}
	else
	{
		if (num > mod_single_line)
		{
			printf("Input error!\n");
			exit(1);
		}
		if (num < 0)
		{
			num += mod_single_line;
		}
		clr_text[k] = alphabet_single_line[num];
	}
}

int encrypt(int n)
{
	//printf("encrypt->is_single_line = %s\n", is_single_line ? "true" : "false");
	if (!is_single_line)
	{
		return n % mod;
	}
	else
	{
		return n % mod_single_line;
	}
}

int decrypt(int n)
{
	//printf("decrypt->is_single_line = %s\n", is_single_line ? "true" : "false");
	if (!is_single_line)
	{
		return n % mod;
	}
	else
	{
		return n % mod_single_line;
	}
}

void encrypt_circular()
{
	//printf("encrypt_circular->is_single_line = %s\n", is_single_line ? "true" : "false");
	for (k = 0; k < (int)strlen(clr_text);)
	{
		for (l = 0; l < key_length; l++, k++)
		{
			if (k >= (int)strlen(clr_text))
			{
				break;
			}
			ciphertext_num[k] = encrypt(cnvrtd_clr_text[k] + cnvrtd_keys[l]);

			//                    if (is_single_line)
			//                        {
			//                            printf("\nClear text = %c(%d),Key = %c(%d),Cipher = %c(%d)\n", alphabet_single_line [cnvrtd_clr_text [k]], cnvrtd_clr_text [k], alphabet_single_line [cnvrtd_keys [l]], cnvrtd_keys [l], alphabet_single_line [ciphertext_num [k]], ciphertext_num [k]);
			//                        }
			//                    else
			//                        {
			//                            printf("\nClear text = %c(%d),Key = %c(%d),Cipher = %c(%d)\n", alphabet [cnvrtd_clr_text [k]], cnvrtd_clr_text [k], alphabet [cnvrtd_keys [l]], cnvrtd_keys [l], alphabet [ciphertext_num [k]], ciphertext_num [k]);
			//                        }

			num_chrctr(ciphertext_num[k]);
		}
	}
	//printf("Ciphertext = %s\n", ciphertext);
}

void decrypt_circular()
{
	//printf("decrypt_circular->is_single_line = %s\n", is_single_line ? "true" : "false");
	for (k = 0; k < (int)strlen(ciphertext);)
	{
		for (l = is_n ? current_key : 0; l < key_length; l++, k++)
		{
			if (k >= (int)strlen(ciphertext))
			{
				break;
			}
			is_n = false;

			clr_text_num[k] = decrypt(cnvrtd_ciphertext[k] - cnvrtd_keys[l]);
			num_chrctr_2(clr_text_num[k]);

			if (ciphertext[k] == '\x0a')
			{
				is_n = true;
				current_key = l == key_length - 1 ? 0 : l + 1;
				//                            k++;
				//                            break;
			}

			//                    if (!is_single_line)
			//                    {
			//                        printf("Current cipher = %c(%d),Key = %c(%d),Clear text = %c(%d),is_n = %s\n", alphabet [cnvrtd_ciphertext [k]], cnvrtd_ciphertext [k], alphabet [cnvrtd_keys [l]], cnvrtd_keys [l], clr_text [k], chrctr_num (clr_text [k]), is_n ? "true" : "false");
			//                    }
			//                    else
			//                    {
			//                        printf("Current cipher = %c(%d),Key = %c(%d),Clear text = %c(%d),is_n = %s\n", alphabet_single_line [cnvrtd_ciphertext [k]], cnvrtd_ciphertext [k], alphabet_single_line [cnvrtd_keys [l]], cnvrtd_keys [l], clr_text [k], chrctr_num (clr_text [k]), is_n ? "true" : "false");
			//                    }

			if (clr_text[k] == '\x0a')
			{
				is_n = false;
				k++;
				break;
			}
		}
	}
}

int main()
{
	//printf("%c\n", alphabet_single_line [14]);
	int op;
	printf("Char set = %d, Current key length = %d\nAll displayable ascii characters are supported\n++ATTENTION++:The type of line break must be DOS(CR/LF)!\n", strlen(alphabet), key_length);
	printf("===============================================================\n1.Encrypt single line from manual input\n2.Decrypt single line from manual input\n3.Encrypt specified file under current path\n4.Decrypt specified file under current path\n5.Exit\n===============================================================\n");
	scanf_s("%d", &op);
	getchar();

	switch (op)
	{
	case 1:
		is_single_line = true;
		printf("Clear text = ");
		gets_s(clr_text);
		for (l = 0; l < (int)strlen(clr_text); l++)
		{
			cnvrtd_clr_text[l] = chrctr_num(clr_text[l]);
			bytes++;
		}
		printf("KEY = ");
		gets_s(keys);
		if (strlen(keys) != key_length)
		{
			printf("The key length doesn't match the setting! Current key length = %d\n", key_length);
			sub();
		}
		is_single_line = true;
		for (l = 0; l < (int)strlen(keys); l++)
		{
			cnvrtd_keys[l] = chrctr_num(keys[l]);
		}
		encrypt_circular();
		printf("Ciphertext = %s\n", ciphertext);
		printf("\n");
		sub();

	case 2:
		is_single_line = true;
		printf("Ciphertext = ");
		gets_s(ciphertext);
		for (l = 0; l < (int)strlen(ciphertext); l++)
		{
			cnvrtd_ciphertext[l] = chrctr_num(ciphertext[l]);
			bytes++;
		}
		printf("KEY = ");
		gets_s(keys);
		if (strlen(keys) != key_length)
		{
			printf("The key length doesn't match the setting! Current key length = %d\n", key_length);
			sub();
		}
		is_single_line = true;
		for (l = 0; l < (int)strlen(keys); l++)
		{
			cnvrtd_keys[l] = chrctr_num(keys[l]);
		}
		decrypt_circular();
		printf("Clear text = %s\n", clr_text);
		printf("\n");
		sub();

	case 3:
	{
		FILE *fp, *fp1;
		unique_ptr<process_bar> pb(new process_bar(35));
		char filename[mx / 1000] = { '\0' }, filename_encrypted[mx / 1000] = { '\0' }, tmp[mx / 1000] = { '\0' };
		system("dir");
		printf("The name of file that need to be encrypted: ");
		gets_s(filename);
		strcpy_s(tmp, filename);
		fopen_s(&fp, filename, "r+");
		if (!fp)
		{
			printf("Failed to open the file \"%s\", please check the existence of it!\n", filename);
			system("pause");
			sub();
		}
		strcat_s(tmp, "_encrypted");
		strcpy_s(filename_encrypted, tmp);
		fopen_s(&fp1, filename_encrypted, "w+");

		printf("KEY = ");
		gets_s(keys);
		if (strlen(keys) != key_length)
		{
			printf("The key length doesn't match the setting! Current key length = %d\n", key_length);
			sub();
		}
		printf("\nEncrypting...\n");
		fseek(fp, 0, SEEK_END);
		long fp_end = ftell(fp);
		//printf("fp_end = %ld\n", fp_end);
		fseek(fp, 0, SEEK_SET);
		//printf("Clear text = \n\n");
		while (fgets(clr_text, mx, fp))
		{
			bytes = 1;
			//printf("%s", clr_text);
			long fp_current = ftell(fp);
			//printf("fp_current = %ld/fp_end = %ld\r\n", fp_current, fp_end);
			//printf("Process: %.2lf%%\r", (double)fp_current / fp_end * 100);
			pb->display(fp_current, fp_end);

			for (l = 0; l < (int)strlen(clr_text); l++)
			{
				cnvrtd_clr_text[l] = chrctr_num(clr_text[l]);
				bytes++;
			}

			for (l = 0; l < (int)strlen(keys); l++)
			{
				cnvrtd_keys[l] = chrctr_num(keys[l]);
			}
			encrypt_circular();
			fputs(ciphertext, fp1);
			line++;
			memset(ciphertext, 0, sizeof(ciphertext));
		}
		if (fclose(fp) != 0)
		{
			printf("Failed to close file \"%s\"!\n", filename);
			system("pause");
			exit(1);
		}
		if (fclose(fp1) != 0)
		{
			printf("Failed to close file \"%s\"!\n", filename_encrypted);
			system("pause");
			exit(1);
		}
		remove(filename);
		printf("\n\nDone. \"%s\" has been created under current path.\nThe \"%s\" has been deleted automatically.\n", filename_encrypted, filename);
		printf("\n");
		sub();
	}
	case 4:
	{
		FILE *fp2, *fp3;
		unique_ptr<process_bar> pb(new process_bar(35));
		char filename[mx / 1000] = { '\0' }, filename_decrypted[mx / 1000] = { '\0' }, tmp[mx / 1000] = { '\0' };
		system("dir");
		printf("The name of file that need to be decrypted: ");
		gets_s(filename);
		strcpy_s(tmp, filename);
		fopen_s(&fp2, filename, "r+");
		if (!fp2)
		{
			printf("Failed to open the file \"%s\", please check the existence of it!\n", filename);
			system("pause");
			sub();
		}
		for (int i = 0; i < (int)strlen(tmp) - 10; i++)
		{
			filename_decrypted[i] = tmp[i];
		}
		fopen_s(&fp3, filename_decrypted, "w+");

		printf("KEY = ");
		gets_s(keys);
		if (strlen(keys) != key_length)
		{
			printf("The key length doesn't match the setting! Current key length = %d\n", key_length);
			sub();
		}
		printf("\nDecrypting...\n");
		//printf("Ciphertext =\n\n");
		fseek(fp2, 0, SEEK_END);
		long fp_end2 = ftell(fp2);
		//printf("fp_end2 = %ld\n", fp_end2);
		fseek(fp2, 0, SEEK_SET);
		//printf("fp_current2 = %ld\n", ftell(*&fp2));
		while (fgets(ciphertext, mx, fp2))
		{
			bytes = 1;
			//printf("%s", ciphertext);
			long fp_current2 = ftell(fp2);
			//printf("fp_current2 = %ld/fp_end2 = %ld\r\n", fp_current2, fp_end2);
			//printf("Process: %.2lf%%\r", (double)fp_current2 / fp_end2 * 100);
			pb->display(fp_current2, fp_end2);

			for (l = 0; l < (int)strlen(ciphertext); l++)
			{
				cnvrtd_ciphertext[l] = chrctr_num(ciphertext[l]);
				bytes++;
			}

			for (l = 0; l < (int)strlen(keys); l++)
			{
				cnvrtd_keys[l] = chrctr_num(keys[l]);
			}
			decrypt_circular();
			fputs(clr_text, fp3);
			line++;
			memset(clr_text, 0, sizeof(clr_text));
		}

		if (fclose(fp2) != 0)
		{
			printf("Failed to close file \"%s\"!\n", filename);
			system("pause");
			exit(1);
		}
		if (fclose(fp3) != 0)
		{
			printf("Failed to close file \"%s\"!\n", filename_decrypted);
			system("pause");
			exit(1);
		}
		printf("\n\nDone. A \"%s\" has been recovered under current path.\n", filename_decrypted);
		printf("\n");
		sub();
	}

	case 5:
		exit(0);

	default:
		printf("Input error!\n");
		system("pause");
		printf("\n");
		sub();
	}
}
