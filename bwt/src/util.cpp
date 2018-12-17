char indexToChar(int &index) {
//    if (index == 0)
//        return '$';
//    if (index == 36)
//        return char(0);
//    if (index == 10)
//        return char(32);
    return char(index);
}

int charToIndex(char c) {
    int index = (int) c;

//    if (index == 0) {
//        return 36;
//    }
//    if (index == 36) {
//        return 0;
//    }
    return index;
}