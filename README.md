# MemFinder OCR
OCR for internet meme images. http://www.memfinder.io/

## Usage

### Learning

Firstly, you need to add the alphabet in the `/extra` directory (e.g. /extra/A.png, /extra/B.png...) of the font you want to learn.

Run the learning:

```bash
make
./learn
```
It will create a `learning` file which contains the neural network features.

### OCR

You need a `learning` file to run the OCR.

Run the OCR:

```bash
make
./ocr <image>
```
