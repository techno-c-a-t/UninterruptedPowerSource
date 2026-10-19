 enum class operatingMode : byte
{
    TRANSMITTING, //0
    CC,
    CV //2

};
enum class activeFunction : byte
{
    VOLTAGE = 1,
    CURRENT,  //2
    ENABLE_OUTPUT,
    DISABLE_OUTPUT, //4
    ENABLE_LOCK,
    DISABLE_LOCK //6
};
struct info
{
    float value;
    operatingMode mode; // 1 - CV, 2 - CC, 0 - TRANSMITTING
    bool outBlocked = 0;
    bool operationSuccess = 0;
    bool safeMode = 0;
    activeFunction function;

};


AnsiString  TBytesToAnsiString(const ::System::Sysutils::TBytes& bytes)
{
    int len = bytes.Length;
    if (len == 0)
    {
        return AnsiString();
    }
    // TBytes.Bytes возвращает указатель на unsigned char*.
    const unsigned char* pRawData = &bytes[0];
    return AnsiString(reinterpret_cast<const char*>(pRawData), len);
}


System::Sysutils::TBytes __fastcall AnsiStringToTBytes(const AnsiString& str)
{
    int len = str.Length();
    ::System::Sysutils::TBytes resultBytes;
    resultBytes.Length = len;
    if (len > 0)
    {
        const char* pAnsiData = str.c_str();
        unsigned char* pBytesData = &resultBytes[0];
        // memcpy(куда_копировать, откуда_копировать, сколько_байт_копировать)
        memcpy(pBytesData, pAnsiData, len);
    }
    return resultBytes;
}



info charBufferToInfo (char* cData, bool dataIsIncomingFromDevice = 1)
{
   info ans;
   if(strlen(cData) != 11)
   {
   throw std::invalid_argument("Error while converting input string to data: string lenth not equal to 11. \n Ошибка конвертации полученной строки в данные: длина строки не равна 11");
   }
   else
   {
        if(cData[0] == 'C')     ans.mode = operatingMode::CC;
        else if (cData[0] == '1')   ans.mode = operatingMode::CV;
        else if ((cData[0] == '0')&&(dataIsIncomingFromDevice == 0))    {ans.mode = operatingMode::TRANSMITTING;}
        else throw std::invalid_argument("Error while converting input string to data: \"adress\" field is not 1 or \"C\". \n Ошибка конвертации полученной строки в данные: поле \"адрес\"  не равно 1 или \"С\" ");
        switch(cData[1])
        {
            case '1':
            ans.operationSuccess = 1;
            case '2':
            ans.function = activeFunction::VOLTAGE;
            break;

            case '3':
            ans.operationSuccess = 1;
            case '4':
            ans.function = activeFunction::CURRENT;
            break;

            case '7':
            ans.outBlocked = 0;
            ans.function = activeFunction::ENABLE_OUTPUT;
            break;
            case '8':
            ans.outBlocked = 1;
            ans.function = activeFunction::DISABLE_OUTPUT;
            break;

            case '9':
            if((cData[2] == 'O')&&(cData[3]== 'K'))   {ans.operationSuccess = 1; break;}
            else if(dataIsIncomingFromDevice == 0)
            {
                if(cData[2] == '1')     ans.safeMode = 1;
                else if(cData[2] == '2')       ans.safeMode = 0;
                else  throw std::invalid_argument("in data function field is 9, but next two fields is not 1 or 2. Поле функции в данных равно 9, но следуюшее поле не 1 и не 2 ");
                break;
            }
            throw std::invalid_argument("in incoming data function field is 9, but next two fields is not \"OK\". Поле функции во входящих данных равно 9, но следующие два поля не \"OK\" ");
            break;
            default:
            throw std::invalid_argument("uncnown function field. Неизвестное значение поля \"функция\" ");
            break;

        }
        return ans;

   }


}
String FormatFloat(float arg)
{
    String ans = FloatToStrF(arg, ffFixed, 7, 4);
    ans.Delete(6,6);
    return ans;

}