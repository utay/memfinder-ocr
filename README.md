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

## References

Very useful pdfs to understand text detection and text recognition in an image:
* http://www.mathematik.uni-marburg.de/~ewerth/papers/ISPA2003.pdf (text detection)
* https://arxiv.org/pdf/1211.4385.pdf (text recognition - artifical neural network)
