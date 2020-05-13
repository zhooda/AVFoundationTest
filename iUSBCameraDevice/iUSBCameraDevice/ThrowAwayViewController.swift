//
//  ThrowAwayViewController.swift
//  iUSBCameraDevice
//
//  Created by Zeeshan Hooda on 5/12/20.
//  Copyright © 2020 Deceptive Labs. All rights reserved.
//

import UIKit
import AVFoundation
 
class ThrowAwayViewController: UIViewController {
 
    // Outlet for our preview
    @IBOutlet weak var previewView: UIView!
 
    // call up our session
    var captureSession: AVCaptureSession?
    var videoPreviewLayer: AVCaptureVideoPreviewLayer?
 
    // this is where we do most of the work
    override func viewDidLoad() {
        super.viewDidLoad()
 
        // Grab AVCaptureDevice class to initialize a device object and define as video
        let captureDevice = AVCaptureDevice.default(for: .video)
 
        do {
            // Grab AVCaptureDeviceInput class using the deivce object we just initialized
            let input = try AVCaptureDeviceInput(device: captureDevice!)
 
            // Initialize the captureSession object
            captureSession = AVCaptureSession()
 
            // Set the input devcie to our capture session
            captureSession?.addInput(input)
 
            // Get the preview layer ready and add under the other layer
            videoPreviewLayer = AVCaptureVideoPreviewLayer(session: captureSession!)
            videoPreviewLayer?.videoGravity = AVLayerVideoGravity.resizeAspectFill
            videoPreviewLayer?.frame = view.layer.bounds
            previewView.layer.addSublayer(videoPreviewLayer!)
 
            // fix our view orientation to match the plist
            let orientation = AVCaptureVideoOrientation.landscapeLeft
            videoPreviewLayer?.connection?.videoOrientation = orientation
 
            // FIRE IT UP
            captureSession?.startRunning()
        } catch {
            // Print errors in case we herp when we should derp
            print(error)
            return
        }
 
    }
 
    // track orientation changes
    func transformOrientation(orientation: UIInterfaceOrientation) -> AVCaptureVideoOrientation {
        switch orientation {
        case .landscapeLeft:
            return .landscapeLeft
        case .landscapeRight:
            return .landscapeRight
        case .portraitUpsideDown:
            return .portraitUpsideDown
        default:
            return .portrait
        }
    }
 
    // hide the dumb status bar
    override var prefersStatusBarHidden: Bool{
        return true
    }
 
    // keep the view from rotating
    override var shouldAutorotate: Bool {
        return false
    }
 
    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
    }
 
}
