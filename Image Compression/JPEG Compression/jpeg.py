import re
import sys
import cv2
import numpy as np
from skimage import io
import warnings

warnings.filterwarnings("ignore")

block = 8
# standard quantization matrix
Quantization = np.array(
    [
        [16, 11, 10, 16, 24, 40, 51, 61],
        [12, 12, 14, 19, 26, 58, 60, 55],
        [14, 13, 16, 24, 40, 57, 69, 56],
        [14, 17, 22, 29, 51, 87, 80, 62],
        [18, 22, 37, 56, 68, 109, 103, 77],
        [24, 35, 55, 64, 81, 104, 113, 92],
        [49, 64, 78, 87, 103, 121, 120, 101],
        [72, 92, 95, 98, 112, 100, 130, 99],
    ],
    dtype=np.float64,
)


def showImage(img):
    io.imshow(img)
    io.show()


def decompress(image, layers, inp):
    for k in range(0, layers):
        if layers > 1:
            img = image[:, :, k]
        else:
            img = image
        # new list for 8x8 sliced image
        DCToutput = []
        # one column of image
        height = len(img)
        # one row of image
        width = len(img[0])
        # dividing block x block parts
        currentY = 0  # current Y index
        for i in range(block, height + 1, block):
            currentX = 0  # current X index
            for j in range(block, width + 1, block):
                # Extracting 128 from all pixels
                DCToutput.append(img[currentY:i, currentX:j])
                currentX = j
            currentY = i
        DCToutput = [np.float64(part) for part in DCToutput]
        invList = []
        for part in DCToutput:
            part -= np.ones((8, 8)) * 128
            part = np.multiply(part, Quantization)
            currentiDCT = cv2.idct(part)
            invList.append(np.int64(currentiDCT))
        row = 0
        rowNcol = []
        for j in range(width // block, len(invList) + 1, width // block):
            rowNcol.append(np.hstack((invList[row:j])))
            row = j
        result = np.vstack((rowNcol))
        result = result + 128
        # print(np.amin(result))
        # print(np.amax(result))
        # clipped to have non-negative integer
        # value ranging from 0 to 255
        result = np.clip(result, 0, 255)
        result = np.uint8(result)
        if layers > 1:
            image[:, :, k] = result
        else:
            image = result
    name = (
        re.findall("(.*)Compressed", inp)[0]
        + "Decompressed"
        + re.findall("[.].*", inp)[0]
    )
    io.imsave(name, image)


def compress(image, layers, inp):
    for k in range(0, layers):
        if layers > 1:
            img = image[:, :, k]
        else:
            img = image
        sliced = []  # new list for 8x8 sliced image
        height = len(img)  # one column of image
        width = len(img[0])  # one row of image
        # dividing block x block parts
        currentY = 0  # current Y index
        for i in range(block, height + 1, block):
            currentX = 0  # current X index
            for j in range(block, width + 1, block):
                sliced.append(
                    img[currentY:i, currentX:j]
                )  # Extracting 128 from all pixels
                currentX = j
            currentY = i
        sliced = [np.float64(img) for img in sliced]
        # DCT on block x block parts
        DCToutput = []
        for part in sliced:
            part -= np.ones((8, 8)) * 128
            currentDCT = cv2.dct(part)
            # Quantization
            currentDCT = np.around(np.divide(currentDCT, Quantization))
            DCToutput.append(currentDCT)
        # Quantization
        # for dct in DCToutput:
        # for i in range(block):
        # for j in range(block):
        # dct[i, j] = np.around(dct[i, j] / Quantization[i, j])
        row = 0
        rowNcol = []
        DCToutput = [np.int64(part) for part in DCToutput]
        for j in range(width // block, len(DCToutput) + 1, width // block):
            rowNcol.append(np.hstack((DCToutput[row:j])))
            row = j
        result = np.vstack((rowNcol))
        result += 128
        result = np.uint8(result)
        if layers > 1:
            image[:, :, k] = result
        else:
            image = result
    name = re.findall("(.*)[.]", inp)[0] + "Compressed" + re.findall("[.].*", inp)[0]
    io.imsave(name, image)


if __name__ == "__main__":
    layers = 1  # default number of layers
    inp = input("Enter image name:")
    img = cv2.imread(inp, cv2.IMREAD_UNCHANGED)
    showImage(img)
    if len(img.shape) >= 3:
        layers = img.shape[2]
        img = img[
            : block * (img.shape[0] // block), : block * (img.shape[1] // block), :
        ]
    else:
        img = img[: block * (img.shape[0] // block), : block * (img.shape[1] // block)]
    if sys.argv[1] == "-c":
        compress(img, layers, inp)
    elif sys.argv[1] == "-d":
        decompress(img, layers, inp)
