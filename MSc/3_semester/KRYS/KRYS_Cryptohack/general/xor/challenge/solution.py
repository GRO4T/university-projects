import cv2

im1 = cv2.imread("flag.png", cv2.IMREAD_COLOR)
im2 = cv2.imread("lemur.png", cv2.IMREAD_COLOR)

result = cv2.bitwise_xor(im2, im1)

cv2.imshow("Bitwise XOR of Image 1 and 2", result)
cv2.waitKey(0)
cv2.destroyAllWindows()
