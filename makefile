COMP = gcc
CFLAGS = -Wall -std=c99
LDFLAGS = `pkg-config allegro-5 allegro_font-5 allegro_ttf-5 allegro_image-5 --libs`

PROGRAM_NAME = jewels

DEP = gameStructure.o gameLogic.o gameState.o

TMP = ./tmp
OBJ = ./obj

all: directories $(DEP)
	@$(COMP) $(CFLAGS) $(OBJ)/* main.c -o $(PROGRAM_NAME) $(LDFLAGS)
	@echo "O executável foi gerado."
	@echo "Para executar, digite: ./$(PROGRAM_NAME)"

# OBJETOS -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

directories:
	@mkdir -p $(OBJ) $(TMP)

gameStructure.o: include/gameStructure/gameStructure.h
	@$(COMP) $(CFLAGS) -c include/gameStructure/gameStructure.c -o $(OBJ)/gameStructure.o

gameState.o: include/gameState/gameState.h
	@$(COMP) $(CFLAGS) -c include/gameState/gameState.c -o $(OBJ)/gameState.o

gameLogic.o: include/gameLogic/gameLogic.h
	@$(COMP) $(CFLAGS) -c include/gameLogic/gameLogic.c -o $(OBJ)/gameLogic.o



# COMANDOS PARA LIMPEZA -=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

clean-obj:
	@rm -rf $(OBJ)
	@echo "Os objetos do programa foram removidos."

clean-tmp:
	@rm -rf $(TMP)
	@echo "Os arquivos temporários foram removidos."

clean: clean-obj clean-tmp
	@rm -f $(PROGRAM_NAME)
	@echo "O executável do programa foi removido."
