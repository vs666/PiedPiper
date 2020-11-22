import re
import sys
from skimage import io
from sklearn.cluster import KMeans
import numpy as np

# Taking input
inp = input("Enter image name:")
print(
    "* Note: The number of colors affects the quality of the image and hence increases the compression."
)
nclusters = int(input("Enter the number of colors:"))
print("* Note: The number of iterations affects the time taken for compression.")
niter = int(input("Enter the number of iterations [25-100]:"))
if niter < 25 or niter > 100:
    print("* ERROR: NUMBER OF ITERATIONS")
    sys.exit()
# Read the image
image = None
try:
    image = io.imread(inp)
except:
    print("* ERROR: IMAGE NOT FOUND")
    sys.exit()
io.imshow(image)
io.show()
# Dimension of the original image
rows = image.shape[0]
cols = image.shape[1]
chls = 1
if len(image.shape) > 2:
    chls = image.shape[2]
# Flatten the image
image = image.reshape(rows * cols, chls)
# Implement k-means clustering to form k clusters
kmeans = KMeans(n_clusters=nclusters, max_iter=niter)
kmeans.fit(image)
# Replace each pixel value with its nearby centroid
compressed_image = kmeans.cluster_centers_[kmeans.labels_]
compressed_image = np.clip(compressed_image.astype("uint8"), 0, 255)
# Reshape the image to original dimension
compressed_image = compressed_image.reshape(rows, cols, chls)
# Save and display output image
# print(re.findall('[.]*', inp))
io.imsave(
    re.findall("(.*)[.]", inp)[0] + "Compressed" + re.findall("[.].*", inp)[0],
    compressed_image,
)
io.imshow(compressed_image)
io.show()
