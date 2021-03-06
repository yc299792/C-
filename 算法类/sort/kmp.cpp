#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void make_next(const char *pattern, int *next) {

	int q, k;
	int m = strlen(pattern);

	next[0] = 0;
	for (q = 1,k = 0;q < m; q ++) {

		while (k > 0 && pattern[q] != pattern[k])
			k = next[k-1];

		if (pattern[q] == pattern[k]) {
			k ++;
		}

		next[q] = k;

	}

}


int kmp(const char *text, const char *pattern, int *next) {

	int n = strlen(text);
	int m = strlen(pattern);

	make_next(pattern, next);
	
	int i, q;
	for (i = 0, q = 0;i < n;i ++) {

		while (q > 0 && pattern[q] != text[i]) {
			q = next[q-1];
		}

		if (pattern[q] == text[i]) {
			q ++;
		}

		if (q == m) {
			//printf("Pattern occurs with shift: %d\n", (i-m+1));
			break;
		}
	}

	return i-q+1;
}

int main() {

	int i;
	int next[20] = {0};

	char *text = "ababxbababababcdababcabddcadfdsss";
	char *pattern = "abcabd";

	int idx = kmp(text, pattern, next);
	printf("match pattern : %d\n", idx);

	for (i = 0;i < strlen(pattern);i ++) {
		printf("%4d", next[i]);
	}
	printf("\n");

	return 0;

}



