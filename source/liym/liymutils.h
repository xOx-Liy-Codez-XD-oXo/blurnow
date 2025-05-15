#ifndef LIYMUTILS_H
#define LIYMUTILS_H

int liym_isNumeric(char c) {
	if (c == '0' || c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7' || c == '8' || c == '9' || c == '.' || c == '-') {
		return 1;
	}
	return 0;
}

int liym_isAlphaNumeric(char c) {
	if (c == '0' || c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7' || c == '8' || c == '9' ||
	    c == 'o' || c == 't' || c == 'n'  || c == 'p' || c == '.' || c == '-') {
		return 1;
	}
	return 0;
}

int liym_isAny(char c) {
	if (c == '0' || c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7' || c == '8' || c == '9' ||
	    c == 'o' || c == 't' || c == 'n'  || c == 'p' || c == '.' || c == '-' || c == ' ') {
		return 1;
	}
	return 0;
}

#define liym_min(a, b) ((a) < (b) ? (a) : (b))
#define liym_max(a, b) ((a) > (b) ? (a) : (b))

#endif