from matplotlib import pyplot as plt

plt.rcParams.update({'font.size': 12})
file_name = input("Enter file name: ")

fileob = open(file_name,'rb')

X = []
a = []
for i in range(256):
    a.append(i)

while True:
    char = fileob.read(1)
    if not char:
        break
    X.append(ord(char))

# fig, ax = plt.subplots(figsize=(25,10))
plt.hist(X,a,range=256)
plt.show()


fileob.close()