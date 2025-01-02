
# **Photoshop**

Jednoduchý a efektivní program v jazyce C pro práci s TGA obrázky, který podporuje základní operace jako změnu velikosti, oříznutí, posun pixelů a převod na stupně šedi. 


---

## **Obsah**

1. [Popis projektu](#popis-projektu)
2. [Funkce programu](#funkce-programu)
3. [Požadavky](#požadavky)
4. [Instalace](#instalace)
5. [Použití](#použití)
6. [Příklady příkazů](#příklady-příkazů)
7. [Autoři](#autoři)

---

## **Popis projektu**+

Tento projekt implementuje jednoduchou verzi Photoshopu pro práci s obrázky ve formátu TGA. Program je napsán v jazyce C a umožňuje provádět různé operace na rastrových obrázcích, jako je změna velikosti, oříznutí, kopírování pixelů nebo převod na stupně šedi.

---

## **Funkce programu**

### Základní funkce:
- **Změna velikosti (`resize`)**: Změní rozměry obrázku na požadovanou šířku a výšku.
- **Oříznutí (`crop`)**: Vybere obdélníkovou část obrázku.
- **Kopírování (`copy`)**: Zkopíruje část obrázku na jinou pozici.
- **Posun (`move`)**: Posune pixely obrázku o zadaný počet pixelů.
- **Převod na stupně šedi (`bw`)**: Převede obrázek na černobílou verzi.
- **Uložení obrázku (`save`)**: Uloží aktuální obrázek do zadaného souboru.
- **Zobrazení informací (`info`)**: Vypíše aktuální rozměry obrázku.

### Podporovaný formát:
- **TGA** (Truevision TGA): Program pracuje s nekomprimovanými RGB nebo černobílými TGA obrázky.

---

## **Požadavky**

### Operační systém:
- Linux (testováno na Ubuntu, kompatibilní s WSL)

### Software:
- GCC (GNU Compiler Collection)
- Make

Instalace závislostí (Linux/WSL):
```bash
sudo apt update
sudo apt install build-essential
```

---

## **Instalace**

1. **Klonování repozitáře:**
   ```bash
   git clone https://github.com/vas-repozitar/photoshop-c.git
   cd photoshop-c
   ```

2. **Překlad programu pomocí `make`:**
   ```bash
   make
   ```

3. **Struktura projektu po překladu:**
   ```
   .
   ├── bin/
   │   └── photoshop          # Spustitelný soubor
   ├── build/
   │   ├── main.o
   │   └── photoshop.o
   ├── img/
   │   ├── img1.tga           # Testovací vstupní soubor
   │   └── output.tga         # Výstupní soubor
   ├── src/
   │   ├── main.c
   │   └── photoshop.c
   │   └── photoshop.h
   ├── Makefile
   └── README.md
   ```

---

## **Použití**

1. **Spusťte program:**
   ```bash
   ./bin/photoshop --input img/img1.tga --output img/output.tga
   ```

2. **Zadejte příkazy do terminálu:**
   Program vás vyzve k zadání příkazů, například:
   ```bash
   > resize 500,500
   > move 10,-10
   > bw
   > save
   > exit
   ```

3. **Zkontrolujte výstupní soubor:**
   Upravený obrázek najdete v souboru `img/output.tga` nebo tam kam ho zadáte v `--output`.

---

## **Příklady příkazů**

| **Příkaz**                | **Popis**                                                                 |
|---------------------------|---------------------------------------------------------------------------|
| `resize 256,256`          | Změní velikost obrázku na 256×256 pixelů.                                |
| `crop 50,50,100,100`      | Ořízne obdélník (50×50) počínaje souřadnicí (50,50).                     |
| `copy 0,0,50,50 100,100`  | Zkopíruje oblast (0,0,50,50) na pozici (100,100).                        |
| `move 10,-10`             | Posune obrázek o 10 pixelů doprava a 10 pixelů nahoru.                   |
| `bw`                      | Převede obrázek na černobílou verzi.                                     |
| `info`                    | Zobrazí aktuální rozměry obrázku.                                        |
| `save`                    | Uloží obrázek do výstupního souboru (`img/output.tga`).                  |
| `exit`                    | Uloží obrázek a ukončí program.                                          |

---


## **Autor**

- **Šimon Ludvíček LUD0046**  
  GitHub: [Můj Github](https://github.com/vas-repozitar)

  

---


