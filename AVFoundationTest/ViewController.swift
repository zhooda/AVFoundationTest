//
//  ViewController.swift
//  AVFoundationTest
//
//  Created by Zeeshan Hooda on 5/11/20.
//  Copyright © 2020 Deceptive Labs. All rights reserved.
//

import Cocoa
import AVFoundation

class ViewController: NSViewController {
    @IBOutlet weak var cameraView: NSView!
    let session: AVCaptureSession = AVCaptureSession()
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
//        switch ([AVCaptureDevice authorizationStatusForMediaType:AVMediaTypeVideo])
//        {
//            case AVAuthorizationStatusAuthorized:
//            {
//                // The user has previously granted access to the camera.
//                [self setupCaptureSession];
//                break;
//            }
//            case AVAuthorizationStatusNotDetermined:
//            {
//                // The app hasn't yet asked the user for camera access.
//                [AVCaptureDevice requestAccessForMediaType:AVMediaTypeVideo completionHandler:^(BOOL granted) {
//                    if (granted) {
//                        [self setupCaptureSession];
//                    }
//                }];
//                break;
//            }
//            case AVAuthorizationStatusDenied:
//            {
//                // The user has previously denied access.
//                return;
//            }
//            case AVAuthorizationStatusRestricted:
//            {
//                // The user can't grant access due to restrictions.
//                return;
//            }
//        }
//        [self setupCaptureSession()]
        self.setupCaptureSession()
        
     
    }
    
    private func setupCaptureSession() {
        // Add camera
         cameraView?.wantsLayer = true
         cameraView?.layer?.backgroundColor = NSColor.black.cgColor
         
         // Session preset
         session.sessionPreset = AVCaptureSession.Preset.low
         
         let input: AVCaptureInput = try! AVCaptureDeviceInput(device: AVCaptureDevice.default(for: .video)!)
         session.addInput(input)
         
         let previewLayer: AVCaptureVideoPreviewLayer = AVCaptureVideoPreviewLayer(session: session)
         previewLayer.frame = cameraView!.bounds
         previewLayer.videoGravity = AVLayerVideoGravity.resizeAspectFill
         
         cameraView?.layer?.addSublayer(previewLayer)
    }
    
    override func viewDidAppear() {
        session.startRunning()
    }
    
    override func viewDidDisappear() {
        session.stopRunning()
    }

    override var representedObject: Any? {
        didSet {
        // Update the view, if already loaded.
        }
    }


}

