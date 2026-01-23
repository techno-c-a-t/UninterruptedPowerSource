class Logger
{
    public:
    int nestLevel = 0;
    std::string filename = "log.txt";
    void handleError(std::string msg);
    std::string getCurrentTime();
    void Logger::log(const std::string msg);

};