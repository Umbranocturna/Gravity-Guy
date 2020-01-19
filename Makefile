build:
		gcc -Wfatal-errors \
		-std=c99 \
		./*.c \
		-I"C:\Development\SDL2 MinGW 64\include" \
		-L"C:\Development\SDL2 MinGW 64\lib" \
		-lmingw32 \
		-lSDL2main \
		-lSDL2 \
		-o example.exe
