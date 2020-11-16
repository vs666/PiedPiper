from PIL import Image, ImageEnhance
from numpy import asarray


# source_path = str(input('Enter Source Path : '))
source_path = 'image.jpg' # debug 
img = Image.open(source_path)
img2 = Image.open(source_path)
print(img.size,'size')
print(img2.getpixel((384,0)))

img = asarray(img)

def pcompress(mat_original):
    # print(len(mat_original),len(mat_original[0]),'lengths')
    if len(mat_original)!=8 or len(mat_original[0])!=8:
        return -1,[0],len(mat_original[0])*len(mat_original)
    Cost = []
    vector_op = []

    # for 0th way
    mean = 0
    elt = 0
    for i in range(8):
        for j in range(8):
            mean += mat_original[i][j]
            elt += 1
    # print(mean,elt)
    mean /= elt
    cost_val = 0
    for i in mat_original:
        for j in i:
            cost_val += (j-mean)**2
    cost_val /= elt
    Cost.append(cost_val)
    vector_op.append([mean])

    # for 1st way
    mean = []
    for j in range(len(mat_original[0])):
        mean.append(0)
    for j in range(len(mat_original[0])):
        elt = 0
        for i in range(len(mat_original)):
            mean[j] = mean[j] + mat_original[i][j]
            elt += 1
        mean[j] /= elt

    cost_val = 0
    for j in range(len(mat_original[0])):
        for i in range(len(mat_original)):
            cost_val += (mat_original[i][j]-mean[j])**2
    cost_val /= (len(mat_original)*len(mat_original[0]))
    Cost.append(cost_val)
    vector_op.append(mean)

    # for 2nd way

    mean = []
    for j in range(len(mat_original)):
        mean.append(0)
    for j in range(len(mat_original)):
        elt = 0
        for i in range(len(mat_original[0])):
            mean[j] = mean[j] + mat_original[j][i]
            elt += 1
        mean[j] /= elt

    cost_val = 0
    for j in range(len(mat_original)):
        for i in range(len(mat_original[0])):
            cost_val += (mat_original[j][i]-mean[j])**2
    cost_val /= (len(mat_original)*len(mat_original[0]))
    Cost.append(cost_val)
    vector_op.append(mean)

    # for 3rd way

    mean = []
    elts = []
    for i in range(len(mat_original)+len(mat_original[0])-1):
        mean.append(0)
        elts.append(0)

    for i in range(len(mat_original)):
        for j in range(len(mat_original[0])):
            mean[7 + i - j] += mat_original[i][j]
            elts[7+i-j] += 1

    for i in range(len(mean)):
        mean[i] /= elts[i]

    cost_val = 0
    for i in range(len(mat_original)):
        for j in range(len(mat_original[0])):
            cost_val += (mat_original[i][j] - mean[7+i-j])**2

    cost_val /= (len(mat_original)*len(mat_original[0]))
    Cost.append(cost_val)
    vector_op.append(mean)

    # for 4th way

    mean = []
    elts = []
    for i in range(len(mat_original)+len(mat_original[0])-1):
        mean.append(0)
        elts.append(0)

    for i in range(len(mat_original)):
        for j in range(len(mat_original[0])):
            mean[7 + 7 - i - j] += mat_original[i][j]
            elts[7 + 7 - i - j] += 1

    for i in range(len(mean)):
        mean[i] /= elts[i]

    cost_val = 0
    for i in range(len(mat_original)):
        for j in range(len(mat_original[0])):
            cost_val += (mat_original[i][j] - mean[7 + 7 - i - j])**2

    cost_val /= (len(mat_original)*len(mat_original[0]))
    Cost.append(cost_val)
    vector_op.append(mean)

    # transformation is i -> 7-i and j-> j
    min_index = 0
    sizes = [2,9,9,16,16]
    for i in range(len(Cost)):
        if Cost[i]<Cost[min_index] or ( Cost[i] == Cost[min_index] and sizes[i] < sizes[min_index]):
            min_index = i
    if Cost[min_index] > 500:
        print("Loss overload")
        return -1,vector_op[min_index],64
    # print('min index = ',min_index)
    return min_index,vector_op[min_index],sizes[min_index]

def makeImage(mean_pixel,way_no,i,j):
    global img2
    global Image

    if i+8 > len(img) or j+8 > len(img[0]):
        # print('Overflow')
        return
    # print(way_no)
    if way_no == 0:
        for x in range(8):
            for y in range(8):
                # print(i+x,j+y,len(img),len(img[0]),i+8,j+8)
                img2.putpixel((j+y,i+x),int(mean_vector[0]))
        
    elif way_no == 1:
        for x in range(8):
            for y in range(8):
                img2.putpixel((j+y,i+x),int(mean_vector[y]))
    
    elif way_no == 2:
        for x in range(8):
            for y in range(8):
                img2.putpixel((j+y,i+x),int(mean_vector[x]))
    
    elif way_no == 3:
        for x in range(8):
            for y in range(8):
                img2.putpixel((j+y,i+x),int(mean_vector[7+x-y]))
    
    elif way_no == 4:
        for x in range(8):
            for y in range(8):
                img2.putpixel((j+y,i+x),int(mean_vector[7+7-x-y]))
    



i, j = 0, 0

total_size = 0

while i < len(img):
    j=0
    while j < len(img[0]):
        flag = False
        # print(i,j,len(img),len(img[0]),total_size)
        matrix_original = []
        for x in range(8):
            if x+i >= len(img):
                flag = True
                break
            temp = []
            for y in range(8):
                if j+y >= len(img[0]):
                    flag = True
                    break
                temp.append(img[i+x][j+y])
            if flag:
                break
            matrix_original.append(temp)
        if flag:
            j+=8
            continue
        bestStroke, mean_vector,sizeCom = pcompress(matrix_original)
        total_size += sizeCom
        if bestStroke != -1:
            makeImage(mean_vector,bestStroke,i,j)
        # else:
        #     print(i+8,j+8,len(img),len(img[0]))
        j+=8
    i+=8

print('Total Compressed size will be : ',total_size)
img2.show()
Image.open(source_path).show()
img2 = img2.save(('compressed_' + source_path))

