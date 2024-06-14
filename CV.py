# import the opencv library
import cv2
import numpy as np
HEADLESS = True

# # get list of available cameras
# for i in range(0, 6):
#     cap = cv2.VideoCapture(i)
#     test, frame = cap.read()
#     print(i, test)
#     cap.release()


# define a video capture object
vid = cv2.VideoCapture(0)
# discard the first few frames to allow the camera to adjust to the lighting
for i in range(10):
    ret, frame = vid.read()

# Create a log file. If the file already exists, it will be overwritten
log_file = open("colorrecognition.log", "w")



while (True):
    # Capture the video frame
    # by frame
    ret, frame = vid.read()

    satbottom = 120
    vbottom = 70
    # Define lower and upper bounds for the color you want to detect
    colornames = ['black', 'blue', 'green', 'red', 'red_top', 'white']
    display_colors = [(60, 60, 60), (255, 0, 0), (0, 255, 0), (0, 0, 255), (0, 0, 255), (255, 255, 255)]
    lower_bound = np.array([[0,0,0],[90,70,0],[45, 75, vbottom],[0,satbottom,vbottom],[165,satbottom,vbottom],[0,0,140]])
    upper_bound = np.array([[180,70,80],[120,256,256],[85, 256, 256], [10, 256, 256], [180, 256, 256], [180, 60, 256]])

    # Cut the top and bottom of the image
    # The camera is 480 x 640 pixels
    frame = frame[1:480, 1:640]

    # Create an array of binary masks of pixels that fall within the specified color range
    hsv_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    masks = [cv2.inRange(hsv_frame, lower_bound[i], upper_bound[i]) for i in range(len(colornames))]

    max_contoursize = [0,0,0,0,0,0];
    total_contoursize = [0,0,0,0,0,0];

    # Find contours
    for i in range(len(colornames)):
        contours = cv2.findContours(masks[i], cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)[0]
        for cnt in contours:
            cv2.drawContours(frame, [cnt], -1, display_colors[i], 2)
        # Save the contour areas
        if len(contours) > 0:
            max_contoursize[i] = max([cv2.contourArea(cnt) for cnt in contours])
            total_contoursize[i] = sum([cv2.contourArea(cnt) for cnt in contours])

    print(max_contoursize)
    print(total_contoursize)
    max_contoursize[3] = max_contoursize[3] + max_contoursize[4] # combine the two reds
    max_contoursize[4] = 0 # remove the second red
    total_contoursize[3] = total_contoursize[3] + total_contoursize[4] # combine the two reds
    total_contoursize[4] = 0 # remove the second red

    print(max_contoursize)
    print(total_contoursize)

    instadominate_edge = 40000
    # Determine the dominant color
    if max(max_contoursize[0:4]) > instadominate_edge: # black and white are excluded from the dominant color
        dominate_color = colornames[max_contoursize.index(max(max_contoursize[0:4]))]
    else:
        dominate_color = colornames[total_contoursize.index(max(total_contoursize))]

    print(dominate_color)

    # Write the dominant color and the max- and total contoursizes to the log file
    log_file.write(dominate_color + ", " + str(max_contoursize) + ", " + str(total_contoursize) + "\n")

    # Write the dominant color to the state file
    state_file = open("color.txt", "w")
    if colornames.index(dominate_color) <= 3:
        state_file.write(str(colornames.index(dominate_color)))
    elif colornames.index(dominate_color) == 5:
        state_file.write("4")
    else:
        state_file.write("5")
    state_file.close()

    # # Filter contours based on area or other criteria
    # min_area = 300
    # filtered_contours = [cnt for cnt in contours if cv2.contourArea(cnt) > min_area]
    #
    # # Get the variance of the color values in the contours
    # color_variance = []
    # for cnt in filtered_contours:
    #     mask = np.zeros(hsv_frame.shape[:2], np.uint8)
    #     cv2.drawContours(mask, [cnt], -1, 255, -1)
    #     mean, std_dev = cv2.meanStdDev(hsv_frame, mask=mask)
    #     color_variance.append(np.var(std_dev))
    #     cv2.drawContours(frame, [cnt], -1, (0, np.var(std_dev)*2, 0), 2)

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
    if not HEADLESS:
        cv2.imshow('frame', frame)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

# After the loop release the camera
vid.release()
log_file.close()
# Destroy all the windows
if not HEADLESS:
    cv2.destroyAllWindows()