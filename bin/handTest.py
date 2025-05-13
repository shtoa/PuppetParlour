import argparse
import enum

import os

import numpy as np
import cv2
import mediapipe as mp

from mediapipe.framework.formats import landmark_pb2


print("testPrint");

from pythonosc import udp_client


from pythonosc.osc_message_builder import OscMessageBuilder

from utils import add_default_args, get_video_input

OSC_ADDRESS = "/mediapipe/hands"
OSC_ADDRESS_2 = "/PID"
labels = [
    "WRIST",
"THUMB_CMC",
"THUMB_MCP",
"THUMB_IP",
"THUMB_TIP",
"INDEX_FINGER_MCP",
"INDEX_FINGER_PIP",
"INDEX_FINGER_DIP",
"INDEX_FINGER_TIP",
"MIDDLE_FINGER_MCP",
"MIDDLE_FINGER_PIP",
"MIDDLE_FINGER_DIP",
"MIDDLE_FINGER_TIP",
"RING_FINGER_MCP",
"RING_FINGER_PIP",
"RING_FINGER_DIP",
"RING_FINGER_TIP",
"PINKY_MCP",
"PINKY_PIP",
"PINKY_DIP",
"PINKY_TIP",
    ];

def send_hands(client: udp_client,
               detections: [landmark_pb2.NormalizedLandmarkList]):
    
    if detections is None:
        client.send_message(OSC_ADDRESS, 0)
        return

    # create message and send
    builder = OscMessageBuilder(address=OSC_ADDRESS)
    
    builder.add_arg(len(detections))
    #builder.address = OSC_ADDRESS+"/"+labels[i];

    for detection in detections:
          
        for i,landmark in zip(np.arange(0,len(detection.landmark),1),detection.landmark):
            
            builder.add_arg(landmark.x)
            builder.add_arg(landmark.y)
            builder.add_arg(landmark.z)
            builder.add_arg(landmark.visibility)
            
     
    msg = builder.build()
    client.send(msg)

def send_pid(client: udp_client):   
     # create message and send
    builder = OscMessageBuilder(address=OSC_ADDRESS_2)
    
    builder.add_arg(1);
    builder.add_arg(os.getpid());

    msg = builder.build();
    client.send(msg);
            
            


def main():
    # read arguments
    print("Starting");
    parser = argparse.ArgumentParser()
    add_default_args(parser)
    args = parser.parse_args()

    # create osc client
    client = udp_client.SimpleUDPClient(args.ip, args.port)

    mp_drawing = mp.solutions.drawing_utils
    mp_hands = mp.solutions.hands

    hands = mp_hands.Hands(
        min_detection_confidence=0.7, min_tracking_confidence=0.5)
    cap = cv2.VideoCapture(get_video_input(args.input))
    
    send_pid(client);

    while cap.isOpened():
        success, image = cap.read()
        if not success:
            print("Failed");
            break

        # Flip the image horizontally for a later selfie-view display, and convert
        # the BGR image to RGB.
        image = cv2.cvtColor(cv2.flip(image, 1), cv2.COLOR_BGR2RGB)
        # To improve performance, optionally mark the image as not writeable to
        # pass by reference.
        image.flags.writeable = False
        results = hands.process(image)

        send_hands(client, results.multi_hand_landmarks)

        # Draw the hand annotations on the image.
        # image.flags.writeable = True
        # image = cv2.cvtColor(image, cv2.COLOR_RGB2BGR)
        # if results.multi_hand_landmarks:
        #     for hand_landmarks in results.multi_hand_landmarks:
        #         mp_drawing.draw_landmarks(
        #             image, hand_landmarks, mp_hands.HAND_CONNECTIONS)
        # cv2.imshow('MediaPipe OSC Hands', image)
        # if cv2.waitKey(5) & 0xFF == 27:
        #     break
    hands.close()
    cap.release()


if __name__ == "__main__":
    print("mainPrint");
    main()
