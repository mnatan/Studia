CC=g++
LD=g++
CFLAGS=-std=c++11 -Wall -c
LDFLAGS=

ifeq (${TARGETARCH}, debug)
  CFLAGS += -g -O0
elif 
  CFLAGS += -O4 -DNDEBUG
endif

SRCFILES=$(wildcard *.cpp)
HEADFILES=$(wildcard *.hpp)
OBJFILES=$(patsubst %.cpp, %.o, ${SRCFILES})
TARGET=test

help:
	@echo "Makefile do kompilacji zadania 3 z ASD"
	@echo ""
	@echo "Sposób użycia: make [ all | headers | main | clean | help ]"
	@echo ""
	@echo "all - kompiluje wszystkie pliki w danym katalogu, .hpp oraz .cpp"
	@echo "\tnastępnie je linkuje i tworzy plik wykonywalny"
	@echo ""
	@echo "headers - kompiluje wszystkie pliki .hpp w katalogu"
	@echo ""
	@echo "main - linkuje wszystkie pliki .o do jednego pliku wykonywalnego"
	@echo ""
	@echo "clean - sprząta katalog z niepotrzebnych plików .o .gch"
	@echo ""
	@echo "help - wyświetla tę informację"
	@echo ""
	@echo ""

all: headers main clean

headers:
	@echo "[Compiling headers] " $@
	${CC} ${CFLAGS} ${HEADFILES}

main: ${OBJFILES}
	@echo "[Linking] "$@
	${LD} ${LDFLAGS} ${OBJFILES} -o ${TARGET}

%.o: %.cpp
	@echo "[Compiling] "$@
	${CC} ${CFLAGS} $^ -o $@

clean:
	@echo "[Cleaning] "
	rm -vrf *.o *.gch
