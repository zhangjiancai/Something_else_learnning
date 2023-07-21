import numpy as np
import matplotlib.pyplot as plt


def gaussian_filter(image_in, kernel_size, sigma):
    """Applies a Gaussian filter to an image.

  Args:
    image_in: The image to be filtered.
    kernel_size: The size of the Gaussian kernel.
    sigma: The standard deviation of the Gaussian kernel.

  Returns:
    The filtered image.
  """

    kernel = np.random.normal(0, sigma, (kernel_size, kernel_size))
    kernel /= np.sum(kernel)

    filtered_image_in_function = np.zeros_like(image_in)
    for i in range(image_in.shape[0] - kernel_size + 1):
        for j in range(image_in.shape[1] - kernel_size + 1):
            filtered_image_in_function[i, j] = np.sum(image_in[i:i + kernel_size, j:j + kernel_size] * kernel)

    return filtered_image_in_function


if __name__ == "__main__":
    image = np.random.randint(0, 255, (100, 100))
    plt.imshow(image)
    filtered_image = gaussian_filter(image, 5, 2)
    plt.imshow(filtered_image)
    print(image.shape)
    print(filtered_image.shape)
