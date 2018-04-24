/** @todo (Itsuki Fujii#1#): Need to be fixed:
*need to be confirmed*1.加密结果为单行时数据过长的截断问题。???
*fixed*2.重复进行加解密时发生的错误。
*fixed*3.fgets()读取密文中换行后第一个字符时发生错误->该字符会被错误地读取为ASCII = 1*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define key_length 5
#define mod 99
#define mx 1000000

int i, k, l, current_key = 0, line = 0;
bool is_n = false;
char alphabet [mx] = "\n\x01\r\t0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ`~!@#$%^&*()-_=+[{]}\\|;:\'\",<.>/\? ", keys [key_length], clr_text [mx], ciphertext [mx], c;
int cnvrtd_clr_text [mx], cnvrtd_keys [key_length], ciphertext_num [mx], cnvrtd_ciphertext [mx], clr_text_num [mx];

void sub()
{
    FILE *sub;
    sub = fopen("sub.dat", "w+");
    char restart [10] = "restart";
    fwrite(restart, sizeof(char), sizeof(restart), sub);
    if (fclose(sub) != 0)
        {
            printf("Failed to close file!\n");
            system("pause");
            exit(1);
        }
    printf("Program has been rebooted.\n");
    system("cipher_sub.exe");
}

int chrctr_num(char c)
{
    for (i = 0; i < mod; i++)
        {
            if ((int) alphabet [i] == (int) c)
                {
                    //printf("\nASCII No. = %d\n", (int) c);
                    return i;
                }
        }
    printf("\n%c not found! ASCII No. = %d, line = %d\n", c, (int) c, line);
    system("pause");
    exit(1);
}

void check(char c)
{
    if ((int) c == 1)
        {
//            printf("got 1!\n");
//            system("pause");
        }
//    else
//        {
//            printf("not 1!\n");
//        }
}

void num_chrctr (int num)
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
    ciphertext [k] = alphabet [num];
    check(ciphertext [k]);
}

void num_chrctr_2 (int num)
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
    clr_text [k] = alphabet [num];
}

int encrypt(int n)
{
    return n % mod;
}

int decrypt(int n)
{
    return n % mod;
}

void encrypt_circular()
{
    for (k = 0; k < (int) strlen(clr_text);)
        {
            for (l = 0; l < key_length; l++, k++)
                {
                    if (k >= (int) strlen(clr_text))
                        {
                            break;
                        }
                    ciphertext_num [k] = encrypt(cnvrtd_clr_text [k] + cnvrtd_keys [l]);
                    //printf("\nClear text = %c(%d),Key = %c(%d),Cipher = %c(%d)\n", alphabet [cnvrtd_clr_text [k]], cnvrtd_clr_text [k], alphabet [cnvrtd_keys [l]], cnvrtd_keys [l], alphabet [ciphertext_num [k]], ciphertext_num [k]);
                    num_chrctr(ciphertext_num [k]);
                }
        }
}

void decrypt_circular()
{
    for (k = 0; k < (int) strlen(ciphertext);)
        {
            for (l = is_n ? current_key : 0; l < key_length; l++, k++)
                {
                    if (k >= (int) strlen(ciphertext))
                        {
                            break;
                        }
                    is_n = false;

                    clr_text_num [k] = decrypt(cnvrtd_ciphertext [k] - cnvrtd_keys [l]);
                    num_chrctr_2(clr_text_num [k]);

                    if (ciphertext [k] == '\n')
                        {
                            is_n = true;
                            current_key = l == key_length - 1 ? 0 : l + 1;
//                            k++;
//                            break;
                        }

                    //printf("Current cipher = %c(%d),Key = %c(%d),Clear text = %c(%d),is_n = %s\n", alphabet [cnvrtd_ciphertext [k]], cnvrtd_ciphertext [k], alphabet [cnvrtd_keys [l]], cnvrtd_keys [l], clr_text [k], chrctr_num (clr_text [k]), is_n ? "true" : "false");

                    if (clr_text [k] == '\n')
                        {
                            is_n = false;
                            k++;
                            break;
                        }
                }
        }
}
// no c, no n, no h,

int main()
{
    int op;
    printf("Char set = %d\nCurrent KEY length = %d\nAll ASCII characters are supported\n", strlen(alphabet), key_length);
    line = 0;
    printf("**************************************************************\n1.Encrypt single line from manual input\n2.Decrypt single line from manual input\n3.Encrypt file text.txt under current path\n4.Decrypt file text_encrypted.txt under current path\n5.Exit\n**************************************************************\n");
    scanf("%d", &op);
    getchar();

    switch (op)
        {
        case 1:
            printf("Clear text = ");
            gets(clr_text);
            for (l = 0; l < (int) strlen(clr_text); l++)
                {
                    cnvrtd_clr_text [l] = chrctr_num(clr_text [l]);
                }
            printf("KEY = ");
            gets(keys);
            for (l = 0; l < (int) strlen(keys); l++)
                {
                    cnvrtd_keys [l] = chrctr_num(keys [l]);
                }
            encrypt_circular();
            printf("Ciphertext = %s\n", ciphertext);
            system("pause");
            printf("\n");
            sub();

        case 2:
            printf("Ciphertext = ");
            gets(ciphertext);
            for (l = 0; l < (int) strlen(ciphertext); l++)
                {
                    cnvrtd_ciphertext [l] = chrctr_num(ciphertext [l]);
                }
            printf("KEY = ");
            gets(keys);
            for (l = 0; l < (int) strlen(keys); l++)
                {
                    cnvrtd_keys [l] = chrctr_num(keys [l]);
                }
            decrypt_circular();
            printf("Clear text = %s\n", clr_text);
            system("pause");
            printf("\n");
            sub();

        case 3:
        {
            FILE *fp, *fp1;
            fp = fopen("text.txt", "r+");
            fp1 = fopen("text_encrypted.txt", "w+");
            if (!fp)
                {
                    printf("Failed to open file!\n");
                    system("pause");
                    exit(1);
                }
            printf("KEY = ");
            gets(keys);
            fseek(fp,0,SEEK_END);
            long fp_end = ftell(fp);
            //printf("fp_end = %ld\n", fp_end);
            fseek(fp,0,SEEK_SET);
            //printf("Clear text = \n\n");
            while(fgets(clr_text, mx, fp))
                {
                    //printf("%s", clr_text);
                    long fp_current = ftell(fp);
                    line++;
                    //printf("fp_current = %ld/fp_end = %ld\r\n", fp_current, fp_end);
                    printf("Process: %.2lf%%\r", (double) fp_current / fp_end * 100);
                    for (l = 0; l < (int) strlen(clr_text); l++)
                        {
                            cnvrtd_clr_text [l] = chrctr_num(clr_text [l]);
                        }

                    for (l = 0; l < (int) strlen(keys); l++)
                        {
                            cnvrtd_keys [l] = chrctr_num(keys [l]);
                        }
                    encrypt_circular();
                    fputs(ciphertext, fp1);
                    memset(ciphertext, 0, sizeof(ciphertext));
                }
            if (fclose(fp) != 0)
                {
                    printf("Failed to close file\n");
                    system("pause");
                    exit(1);
                }
            if (fclose(fp1) != 0)
                {
                    printf("Failed to close file\n");
                    system("pause");
                    exit(1);
                }
            remove("text.txt");
            printf("\n\nDone.A \"text_encrypted.txt\" has been created under current path.\nThe \"text.txt\" has been deleted automatically.\n");
            system("pause");
            printf("\n");
            sub();
        }
        case 4:
        {
            FILE *fp2, *fp3;
            fp2 = fopen("text_encrypted.txt", "r+");
            fp3 = fopen("text.txt", "w+");
            if (!fp2)
                {
                    printf("Failed to open file!\n");
                    system("pause");
                    exit(1);
                }
            printf("KEY = ");
            gets(keys);
            //printf("Ciphertext =\n\n");
            fseek(fp2,0,SEEK_END);
            long fp_end2 = ftell(fp2);
            //printf("fp_end2 = %ld\n", fp_end2);
            fseek(fp2,0,SEEK_SET);
            //printf("fp_current2 = %ld\n", ftell(fp2));
            while(fgets(ciphertext, mx, fp2))
                {
                    //printf("%s", ciphertext);
                    long fp_current2 = ftell(fp2);
                    line++;
                    //printf("fp_current2 = %ld/fp_end2 = %ld\r\n", fp_current2, fp_end2);
                    printf("Process: %.2lf%%\r", (double) fp_current2 / fp_end2 * 100);

                    for (l = 0; l < (int) strlen(ciphertext); l++)
                        {
                            cnvrtd_ciphertext [l] = chrctr_num(ciphertext [l]);
                        }

                    for (l = 0; l < (int) strlen(keys); l++)
                        {
                            cnvrtd_keys [l] = chrctr_num(keys [l]);
                        }
                    decrypt_circular();
                    fputs(clr_text, fp3);
                    memset(clr_text, 0, sizeof(clr_text));
                }

            if (fclose(fp2) != 0)
                {
                    printf("Failed to close file\n");
                    system("pause");
                    exit(1);
                }
            if (fclose(fp3) != 0)
                {
                    printf("Failed to close file\n");
                    system("pause");
                    exit(1);
                }
            //remove("text_encrypted.txt");
            printf("\n\nDone.A \"text.txt\" has been recovered under current path.\n");
            system("pause");
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
