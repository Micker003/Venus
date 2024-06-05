# import the opencv library
import cv2
import numpy as np

# define a video capture object
vid = cv2.VideoCapture(0)

while (True):

    # Capture the video frame
    # by frame
    ret, frame = vid.read()


    # Define lower and upper bounds for the color you want to detect
    lower_bound = np.array([40, 100, 100])
    upper_bound = np.array([100, 230, 230])

    # # Define lower and upper bounds for the color you want to detect in RGB
    # lower_bound_rgb = np.array([0, 0, 0])
    # upper_bound_rgb = np.array([255, 255, 255])

    # Create a binary mask of pixels that fall within the specified color range
    hsv_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    mask = cv2.inRange(hsv_frame, lower_bound, upper_bound)

    # Find contours
    contours, _ = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

    # Filter contours based on area or other criteria
    min_area = 300
    filtered_contours = [cnt for cnt in contours if cv2.contourArea(cnt) > min_area]

    # Get the variance of the color values in the contours
    color_variance = []
    for cnt in filtered_contours:
        mask = np.zeros(hsv_frame.shape[:2], np.uint8)
        cv2.drawContours(mask, [cnt], -1, 255, -1)
        mean, std_dev = cv2.meanStdDev(hsv_frame, mask=mask)
        color_variance.append(np.var(std_dev))
        cv2.drawContours(frame, [cnt], -1, (0, np.var(std_dev)*2, 0), 2)

    # Create a seperate set depending on the variance of the color values, the higher the variance the more colorful the contour
    # for cnt in filtered_contours:
    #     mask = np.zeros(frame.shape[:2], np.uint8)
    #     cv2.drawContours(mask, [cnt], -1, 255, -1)
    #     mean, std_dev = cv2.meanStdDev(frame, mask=mask)
    #     color_variance.append(np.var(std_dev))

    # # Create a seperate set depending on how square the contour is
    # square_contours = []
    # non_square_contours = []
    # for cnt in filtered_contours:
    #     x, y, w, h = cv2.boundingRect(cnt)
    #     aspect_ratio = w / h
    #     if 0.9 <= aspect_ratio <= 1.1:
    #         square_contours.append(cnt)
    #     else:
    #         non_square_contours.append(cnt)
    #
    # # Draw contours on the original image
    # cv2.drawContours(frame, filtered_contours, -1, (0, 255, 0), 2)
    # cv2.drawContours(frame, square_contours, -1, (255, 0, 0), 2)

    # Display the resulting frame
    cv2.imshow('frame', frame)

    # the 'q' button is set as the
    # quitting button you may use any
    # desired button of your choice
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# After the loop release the cap object
vid.release()
# Destroy all the windows
cv2.destroyAllWindows()