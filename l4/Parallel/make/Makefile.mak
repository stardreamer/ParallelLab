#Makefile
SRCDIR := source

OBJDIR := obj

BINDIR := bin

CC := mpicc
FLAGS := -Wall -pedantic -ffreestanding -Wstrict-overflow=4 -Wformat-security -Wextra -Wfloat-equal -Wundef -Wshadow -O2  -c -std=c99 -MD
BINDIR_REL := $(addprefix ../, $(BINDIR))

SRCDIR_REL := $(addprefix ../, $(SRCDIR))

SRCFILES := $(wildcard $(addsuffix /*.c, $(SRCDIR_REL)))

OBJDIR_REL := $(addprefix ../, $(OBJDIR))

OBJFILES := $(patsubst $(SRCDIR_REL)/%.c,$(OBJDIR_REL)/%.o,$(wildcard $(SRCFILES)))

default:
	@echo -e "\e[0;33mmake mpimatrix\e[0;32m - компиляция сервера"
	@echo -e "\e[0;33mmake clean\e[0;32m - чистка объектных файлов"
	@echo -e "\e[0;33mmake delete\e[0;32m - удаление исполняемых файлов" 
	
mpiSort:$(BINDIR_REL)/mpiSort $(OBJFILES)  
	@echo -e "Компиляция mpi программы успешна\n"
	

$(BINDIR_REL)/mpiSort: $(OBJFILES) 
	$(CC) -Wall $^ -o $@ -lm

	

$(OBJDIR_REL)/%.o: $(SRCDIR_REL)/%.c
	$(CC) $<  $(FLAGS) $(addprefix -I, $(SRCDIR_REL)) -o $@ -pipe -lm
	
include  $(wildcard $(OBJDIR_REL)/*.d)
#Все пользователи имеют право читать копируется в каталог
clean:
	@rm -f $(OBJDIR_REL)/*.o $(OBJDIR_REL)/*.d
	@rm -f $(OBJDIR_REL_S)/*.o $(OBJDIR_REL_S)/*.d
	@rm -f $(OBJDIR_REL_D)/*.o $(OBJDIR_REL_D)/*.d
	@rm -f $(BINDIR_REL)/*
	@echo "Очистка завершена!"
delete:
	@echo "Клиент удалён!"
	@rm -f $(BINDIR_REL)
	@echo "Сервер удалён!"
.PHONY: clean delete mpiSort
