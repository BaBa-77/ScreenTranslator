double strtod(const char *s00, char **se);

double atof(const char *nptr) {
    return (strtod(nptr, (char **) 0));
}
