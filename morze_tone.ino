// Определяем пин, к которому подключен динамик (или пьезоэлемент)
const int SPEAKER_PIN = 8; 

// Частота звука для азбуки Морзе (в герцах). 
// 700-800 Гц — стандартный и самый приятный для слуха радиолюбительский тон.
const int MORSE_FREQ = 800;

// Настройка длительностей сигналов (в миллисекундах)
const int DOT_DURATION = 200;    // Длительность точки (.)
const int DASH_DURATION = 600;   // Длительность тире (-)
const int SYMBOL_PAUSE = 200;    // Пауза между точками и тире внутри одной буквы
const int LETTER_PAUSE = 600;    // Пауза между разными буквами

void setup() {
  // Инициализируем Serial-порт на скорости 9600 бод
  Serial.begin(9600);
  
  // Настраиваем пин динамика как выход
  pinMode(SPEAKER_PIN, OUTPUT);
  
  // Приглашение к вводу
  Serial.println("========================================");
  Serial.println("Интерактивный звуковой переводчик Морзе");
  Serial.println("Введите текст (латиница, кириллица, цифры):");
  Serial.println("========================================");
}

void loop() {
  // Проверяем, есть ли данные в буфере Serial-порта
  if (Serial.available() > 0) {
    // Читаем всю введенную строку до символа перевода строки ('\n')
    String input = Serial.readStringUntil('\n');
    
    input.trim();          // Удаляем лишние пробелы и переносы по краям
    input.toLowerCase();   // Переводим все символы в нижний регистр
    
    Serial.print("Текст: ");
    Serial.println(input);
    Serial.print("Морзе: ");

    // Проходим по строке побайтово, учитывая двухбайтовую кодировку UTF-8
    int i = 0;
    while (i < input.length()) {
      char c1 = input[i];
      
      // Если старший бит равен 0 — это латиница или цифра (1 байт)
      if ((c1 & 0x80) == 0) {
        printAndToneMorseChar(c1);
        i++;
      } 
      else {
        // Если старший бит 1 — это кириллица (2 байта)
        char c2 = input[i + 1];
        printAndToneMorseUtf8(c1, c2);
        i += 2;
      }
      
      Serial.print(" ");      // Пробел между буквами в консоли
      delay(LETTER_PAUSE);    // Пауза звука между буквами
    }
    
    Serial.println("\n----------------------------------------");
  }
}

// ==========================================
// Обработка латиницы (a-z) и цифр (0-9)
// ==========================================
void printAndToneMorseChar(char c) {
  switch (c) {
    // Латиница
    case 'a': playToneSequence(".-"); break;
    case 'b': playToneSequence("-..."); break;
    case 'c': playToneSequence("-.-."); break;
    case 'd': playToneSequence("-.."); break;
    case 'e': playToneSequence("."); break;
    case 'f': playToneSequence("..-."); break;
    case 'g': playToneSequence("--."); break;
    case 'h': playToneSequence("...."); break;
    case 'i': playToneSequence(".."); break;
    case 'j': playToneSequence(".---"); break;
    case 'k': playToneSequence("-.-"); break;
    case 'l': playToneSequence(".-.."); break;
    case 'm': playToneSequence("--"); break;
    case 'n': playToneSequence("-."); break;
    case 'o': playToneSequence("---"); break;
    case 'p': playToneSequence(".--."); break;
    case 'q': playToneSequence("--.-"); break;
    case 'r': playToneSequence(".-."); break;
    case 's': playToneSequence("..."); break;
    case 't': playToneSequence("-"); break;
    case 'u': playToneSequence("..-"); break;
    case 'v': playToneSequence("...-"); break;
    case 'w': playToneSequence(".--"); break;
    case 'x': playToneSequence("-..-"); break;
    case 'y': playToneSequence("-.--"); break;
    case 'z': playToneSequence("--.."); break;
    
    // Цифры
    case '1': playToneSequence(".----"); break;
    case '2': playToneSequence("..---"); break;
    case '3': playToneSequence("...--"); break;
    case '4': playToneSequence("....-"); break;
    case '5': playToneSequence("....."); break;
    case '6': playToneSequence("-...."); break;
    case '7': playToneSequence("--..."); break;
    case '8': playToneSequence("---.."); break;
    case '9': playToneSequence("----."); break;
    case '0': playToneSequence("-----"); break;
    
    // Пробелы и спецсимволы
    default: 
      Serial.print("/"); 
      delay(LETTER_PAUSE); 
      break; 
  }
}

// ==========================================
// Обработка кириллицы (русские и украинские буквы в UTF-8)
// ==========================================
void printAndToneMorseUtf8(unsigned char c1, unsigned char c2) {
  if (c1 == 0xD0) {
    switch (c2) {
      case 0xB0: playToneSequence(".-"); break;     // а
      case 0xB1: playToneSequence("-..."); break;   // б
      case 0xB2: playToneSequence(".--."); break;   // в
      case 0xB3: playToneSequence("--."); break;    // г
      case 0xB4: playToneSequence("-.."); break;    // д
      case 0xB5: playToneSequence("."); break;      // е
      case 0xB6: playToneSequence("...-"); break;   // ж
      case 0xB7: playToneSequence("--.."); break;   // з
      case 0xB8: playToneSequence(".."); break;     // и
      case 0xB9: playToneSequence(".---"); break;   // й
      case 0xBA: playToneSequence("-.-"); break;    // к
      case 0xBB: playToneSequence(".-.."); break;   // л
      case 0xBC: playToneSequence("--"); break;     // м
      case 0xBD: playToneSequence("-."); break;     // н
      case 0xBE: playToneSequence("---"); break;    // о
      case 0xBF: playToneSequence(".--."); break;   // п
      case 0x84: playToneSequence("..-.."); break;  // є
      case 0x86: playToneSequence(".."); break;     // і
    }
  } 
  else if (c1 == 0xD1) {
    switch (c2) {
      case 0x80: playToneSequence(".-."); break;    // р
      case 0x81: playToneSequence("..."); break;    // с
      case 0x82: playToneSequence("-"); break;      // т
      case 0x83: playToneSequence("..-"); break;    // у
      case 0x84: playToneSequence("..-."); break;   // ф
      case 0x85: playToneSequence("...."); break;   // х
      case 0x86: playToneSequence("-.-."); break;   // ц
      case 0x87: playToneSequence("---."); break;   // ч
      case 0x88: playToneSequence("----"); break;   // ш
      case 0x89: playToneSequence("--.-"); break;   // щ
      case 0x8A: playToneSequence("-.--."); break;  // ъ
      case 0x8B: playToneSequence("-.--"); break;   // ы
      case 0x8C: playToneSequence("-..-"); break;   // ь
      case 0x8D: playToneSequence("..-.."); break;  // э
      case 0x8E: playToneSequence("..-"); break;    // ю
      case 0x8F: playToneSequence(".-.-"); break;   // я
      case 0x97: playToneSequence(".--."); break;   // ї
    }
  } 
  else if (c1 == 0xD2) {
    switch (c2) {
      case 0x91: playToneSequence("--."); break;    // ґ
    }
  }
}

// ==========================================
// Функция вывода в консоль и генерации звука
// ==========================================
void playToneSequence(const char* morseCode) {
  // Выводим последовательность в Serial Monitor
  Serial.print(morseCode);
  
  // Посимвольно разбираем строку кода Морзе
  for (int i = 0; morseCode[i] != '\0'; i++) {
    char symbol = morseCode[i];
    
    if (symbol == '.') {
      // Включаем чистый тон на заданной частоте для точки
      tone(SPEAKER_PIN, MORSE_FREQ, DOT_DURATION);
      delay(DOT_DURATION);
    } 
    else if (symbol == '-') {
      // Включаем чистый тон на заданной частоте для тире
      tone(SPEAKER_PIN, MORSE_FREQ, DASH_DURATION);
      delay(DASH_DURATION);
    }
    
    // Принудительно выключаем звук перед паузой
    noTone(SPEAKER_PIN);
    delay(SYMBOL_PAUSE);         // Пауза между точками/тире внутри знака
  }
}
