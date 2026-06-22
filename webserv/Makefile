NAME        = webserv
CXX         = c++
CXXFLAGS    = -std=c++98

SRCS        = tcpservercopy.cpp ConfigParser.cpp ConfigServer.cpp Location.cpp Client.cpp Server.cpp Response.cpp cgi.cpp
OBJS        = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
