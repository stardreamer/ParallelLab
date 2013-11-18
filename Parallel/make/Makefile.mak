#Makefile
SRCDIR := source

OBJDIR := obj

BINDIR := bin

CC := mpicc

BINDIR_REL := $(addprefix ../, $(BINDIR))

SRCDIR_REL := $(addprefix ../, $(SRCDIR))

SRCFILES := $(wildcard $(addsuffix /*.c, $(SRCDIR_REL)))

OBJDIR_REL := $(addprefix ../, $(OBJDIR))

OBJFILES := $(patsubst $(SRCDIR_REL)/%.c,$(OBJDIR_REL)/%.o,$(wildcard $(SRCFILES)))

default:
	@echo -e "\e[0;33mmake mpimatrix\e[0;32m - компиляция сервера"
	@echo -e "\e[0;33mmake clean\e[0;32m - чистка объектных файлов"
	@echo -e "\e[0;33mmake delete\e[0;32m - удаление исполняемых файлов" 
	
mpimatrix:$(BINDIR_REL)/mpimatrix $(OBJFILES)  
	@echo -e "Компиляция mpi программы успешна\n"
	

$(BINDIR_REL)/mpimatrix: $(OBJFILES) 
	$(CC) -Wall -lm $^ -o $@

	

$(OBJDIR_REL)/%.o: $(SRCDIR_REL)/%.c
	$(CC) $<  -Wall -pedantic -lm -c -std=c99 -MD $(addprefix -I, $(SRCDIR_REL)) -o $@ -pipe
	
include  $(wildcard $(OBJDIR_REL)/*.d)
#Все пользователи имеют право читать копируется в каталог
clean:
	@rm -f $(OBJDIR_REL)/*.o $(OBJDIR_REL)/*.d
	@rm -f $(OBJDIR_REL_S)/*.o $(OBJDIR_REL_S)/*.d
	@rm -f $(OBJDIR_REL_D)/*.o $(OBJDIR_REL_D)/*.d
	@echo "Очистка завершена!"
delete:
	@echo "Клиент удалён!"
	@rm -f $(BINDIR_REL)
	@echo "Сервер удалён!"
.PHONY: clean delete mpimatrix
