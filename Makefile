# Define o compilador C++ a ser usado
CXX = g++

# Define as flags de compilação
# -std=c++11: Usa o padrão C++11
# -Wall: Ativa a maioria dos avisos do compilador (boa prática)
# -g: Gera informações de depuração (útil para usar com gdb)
CXXFLAGS = -std=c++11 -Wall -g

# Define o nome do executável final
TARGET = simulator

# Lista todos os arquivos-fonte (.cpp) do projeto
SRCS = main.cpp Simulator.cpp

# Gera automaticamente a lista de arquivos-objeto (.o) a partir dos arquivos-fonte
# Ex: main.cpp Simulator.cpp -> main.o Simulator.o
OBJS = $(SRCS:.cpp=.o)

# A regra 'all' é a regra padrão, que será executada se você apenas digitar 'make'
# Ela depende do executável final.
all: $(TARGET)

# Regra para vincular (link) os arquivos-objeto e criar o executável final
# Depende de todos os arquivos-objeto.
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Regra genérica para compilar um arquivo .cpp em um arquivo .o
# O comando é executado sempre que um arquivo .o é necessário e o .cpp correspondente foi modificado.
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Regra 'clean' para limpar os arquivos gerados pela compilação
clean:
	rm -f $(TARGET) $(OBJS)

run:
	./simulator

# Declara que 'all' e 'clean' não são nomes de arquivos
.PHONY: all clean
