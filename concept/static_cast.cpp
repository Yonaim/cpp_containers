// 컴파일 타임에 형변환(type-conversion) 오류를 잡아준다.

struct myType
{
};

int main()
{
    int     num = 42;
    myType *ptr1 = (myType *)(&num); // compile Errors 인식 X
    // myType *ptr2 = static_cast<myType *>(&num); // compile Error 인식 O
}
