//
//  ViewController.swift
//  Test2
//
//  Created by Mark Pizzutillo on 11/3/18.
//  Copyright © 2018 Mark Pizzutillo. All rights reserved.
//

import UIKit
import CocoaAsyncSocket

class ViewController: UIViewController, GCDAsyncUdpSocketDelegate, GCDAsyncSocketDelegate {
    
    @IBOutlet weak var targetTempLabel: UILabel!
    
    @IBOutlet weak var sliderControl: UISlider!
    
    @IBOutlet weak var currentTempLabel: UILabel!
    
    var address = "192.168.4.1"
    var port:UInt16 = 4211
    
    var socket : GCDAsyncUdpSocket!
    var recvSocket : GCDAsyncSocket!
    
    let alert = UIAlertController(title: nil, message: "Connecting...", preferredStyle: .alert)
    
    struct stats {
        var targetTemp: Int16
        var currentTemp: Int16
        var hotValveClosed: Bool
        var coldValveClosed: Bool
        var grayWaterClosed: Bool
        
        init() {
            currentTemp = 0
            targetTemp = 0
            hotValveClosed = false
            coldValveClosed = false
            grayWaterClosed = true
        }
    }
    
    var targetTemp:Int16 = 50 {
        didSet {
            targetTempLabel.text = String(targetTemp) + " C"
        }
    }
    
    var currentTemp:Int16 = 50 {
        didSet {
            currentTempLabel.text = String(currentTemp) + " C"
        }
    }
    
    var statuses = stats()

    override func viewDidAppear(_ animated: Bool) {
        super.viewDidAppear(animated)
        
        let loadingIndicator = UIActivityIndicatorView(frame: CGRect(x: 10, y: 5, width: 50, height: 50))
        loadingIndicator.hidesWhenStopped = true
        loadingIndicator.activityIndicatorViewStyle = UIActivityIndicatorViewStyle.gray
        loadingIndicator.startAnimating();
        
        alert.view.addSubview(loadingIndicator)
        
        present(alert, animated: true, completion: nil)
        
    }
    override func viewDidLoad() {
        super.viewDidLoad()
        
        let message = "HelloFromPhoneApp".data(using: String.Encoding.utf8)
        
        socket = GCDAsyncUdpSocket(delegate: self, delegateQueue: DispatchQueue.main)
        
        
        do {
            try socket.bind(toPort: 4210)
            //try socket.acceptOnPort(4210)
            try socket.beginReceiving()
            try socket.enableBroadcast(true)
            socket.send(message!, toHost: address, port: port, withTimeout: 1000, tag: 0)
            
        } catch {
            print(error)
        }
        
        // Setup the other socket (used to handle the response from the Harmony hub)
        recvSocket = GCDAsyncSocket(delegate: self,  delegateQueue:DispatchQueue.main)
        
        do {
            
            // Accept connections on port 57612
            try recvSocket.accept(onPort: 4210)
            
        } catch {
            
            // Handle any errors here
            print(error)
        }
        
    }
    
    @IBAction func sliderValueChanged(_ sender: Any) {
        let slider = sender as! UISlider
        
        targetTemp = Int16(slider.value)
    }
    
    @IBAction func doneChangingTarget(_ sender: Any) {
        print("Done")
    }
    

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    
    
    func udpSocket(_ sock: GCDAsyncUdpSocket, didConnectToAddress address: Data) {
        print("didConnectToAddress");
    }
    
    func udpSocket(_ sock: GCDAsyncUdpSocket, didNotConnect error: Error?) {
        print("didNotConnect \(String(describing: error))")
    }
    
    func udpSocket(_ sock: GCDAsyncUdpSocket, didSendDataWithTag tag: Int) {
        print("didSendDataWithTag")
    }
    
    func udpSocket(_ sock: GCDAsyncUdpSocket, didNotSendDataWithTag tag: Int, dueToError error: Error?) {
        print("didNotSendDataWithTag")
    }
    
    func udpSocket(_ sock: GCDAsyncUdpSocket, didReceive data: Data, fromAddress address: Data, withFilterContext filterContext: Any?) {
        //let gotdata: NSString = NSString(data: data, encoding: String.Encoding.utf8.rawValue)!
        
        statuses = data.withUnsafeBytes { $0.pointee as stats }
        
        targetTemp = statuses.targetTemp
        currentTemp = statuses.currentTemp
        alert.dismiss(animated: true, completion: nil)
    }
    
    func socket(_ sock: GCDAsyncSocket, didRead data: Data, withTag tag: Int) {
        let gotdata: NSString = NSString(data: data, encoding: String.Encoding.utf8.rawValue)!
        print(gotdata)
        print("recvd2")
    }
}

