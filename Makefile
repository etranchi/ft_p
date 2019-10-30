CLIENT = client
SERVER = server 


FLAGS= -Wall -Werror -Wextra
INC = -I include/
CC = gcc

SRCS_CLIENT = client_s/client.c client_s/perform_cmd.c utils/utils.c utils/free_all.c utils/utils_fd.c utils/utils_pwd.c
OBJ_CLIENT =  $(SRCS_CLIENT:.c=.o)

SRCS_SERVER = server_s/server.c server_s/perform_server_cmd.c utils/utils.c utils/free_all.c utils/utils_fd.c utils/utils_pwd.c
OBJ_SERVER =  $(SRCS_SERVER:.c=.o)

LIBFT = -L lib/libft/ -lft

all: $(CLIENT) $(SERVER)

$(CLIENT) : $(OBJ_CLIENT)
	@make -C lib/libft/
	$(CC) $(FLAGS) $(INC) -o $(CLIENT) $(OBJ_CLIENT) $(LIBFT)

$(SERVER) : $(OBJ_SERVER)
	$(CC) $(FLAGS) $(INC) -o $(SERVER) $(OBJ_SERVER) $(LIBFT)

%.o: %.c
	@$(CC) $(FLAGS) -c $< -o $@

clean:
	@ /bin/rm -f $(OBJ_CLIENT)
	@ /bin/rm -f $(OBJ_SERVER)
	@ make -C lib/libft clean

fclean: clean
	@ /bin/rm -f $(CLIENT)
	@ /bin/rm -f $(SERVER)
	@ make -C lib/libft fclean

re: fclean all

.PHONY: all, clean, fclean, re