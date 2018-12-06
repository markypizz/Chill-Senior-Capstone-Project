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
    
    @IBOutlet weak var mainValveLabel: UILabel!
    
    @IBOutlet weak var coldValveLabel: UILabel!
    
    @IBOutlet weak var showerReadyLabel: UILabel!
    
    @IBOutlet weak var alert1Label: UILabel!
    
    @IBOutlet weak var alert2Label: UILabel!
    
    @IBOutlet weak var alert1Stepper: UIStepper!
    
    @IBOutlet weak var alert2Stepper: UIStepper!
    
    @IBOutlet weak var confirmButton: UIButton!
    
    
    var alert1Time : Int32 = 5 {
        didSet {
            alert1Label.text = "Alert 1: \(alert1Time) minutes"
            alert1Stepper.value = Double((alert1Time))
            responses.timer1 = alert1Time
        }
        
    }
    
    var alert2Time : Int32 = 3 {
        didSet {
            alert2Label.text = "Alert 2: \(alert2Time) minutes"
            alert2Stepper.value = Double((alert2Time))
            responses.timer2 = alert2Time
        }
    }
    
    var address = "192.168.4.1"
    var port:UInt16 = 4211
    
    var socket : GCDAsyncUdpSocket!
    var sendSocket : GCDAsyncUdpSocket!
    //var recvSocket : GCDAsyncSocket!
    
    let alert = UIAlertController(title: nil, message: "Connecting...", preferredStyle: .alert)
    
    struct stats {
        var timer1: Int32
        var timer2: Int32
        var targetTemp: Int16
        var currentTemp: Int16
        var servoAngle: UInt8
        var mainValveClosed: Bool
        var showerReady: Bool
        
        init() {
            currentTemp = 0
            targetTemp = 0
            timer1 = 5
            timer2 = 3
            mainValveClosed = false
            servoAngle = 180
            showerReady = false
        }
    }
    
    struct responseStats : Codable {
        var timer1 : Int32
        var timer2 : Int32
        var targetTemp : Int16
        init() {
            timer1 = 5
            timer2 = 3
            targetTemp = 35
        }
        
        static func archive(w:responseStats) -> Data {
            var fw = w
            return Data(bytes: &fw, count: MemoryLayout<responseStats>.stride)
        }
        
        static func unarchive(d:Data) -> responseStats {
            guard d.count == MemoryLayout<responseStats>.stride else {
                fatalError("BOOM!")
            }
            
            var w:responseStats?
            d.withUnsafeBytes({(bytes: UnsafePointer<responseStats>)->Void in
                w = UnsafePointer<responseStats>(bytes).pointee
            })
            return w!
        }
    }
    
    var targetTemp:Int16 = 50 {
        didSet {
            targetTempLabel.text = String(targetTemp) + " C"
            responses.targetTemp = targetTemp
        }
    }
    
    var currentTemp:Int16 = 50 {
        didSet {
            currentTempLabel.text = String(currentTemp) + " C"
        }
    }
    
    var statuses = stats()
    var responses = responseStats()
    var expectingUpdate : Bool = true

    override func viewDidAppear(_ animated: Bool) {
        super.viewDidAppear(animated)
        
        let loadingIndicator = UIActivityIndicatorView(frame: CGRect(x: 10, y: 5, width: 50, height: 50))
        loadingIndicator.hidesWhenStopped = true
        loadingIndicator.activityIndicatorViewStyle = UIActivityIndicatorViewStyle.gray
        loadingIndicator.startAnimating();
        
        alert.view.addSubview(loadingIndicator)
        
        //Remove this line to remove waiting screen (for testing)
        present(alert, animated: true, completion: nil)
        
    }
    override func viewDidLoad() {
        super.viewDidLoad()
        confirmButton.isEnabled = true
        socket = GCDAsyncUdpSocket(delegate: self, delegateQueue: DispatchQueue.main)
        sendSocket = GCDAsyncUdpSocket(delegate: self, delegateQueue: DispatchQueue.main)
        
        do {
            try socket.bind(toPort: 4210)
            //try socket.acceptOnPort(4210)
            try socket.beginReceiving()
            try socket.enableBroadcast(true)
            //socket.send(message!, toHost: address, port: port, withTimeout: 1000, tag: 0)
        } catch {
            print(error)
        }
        
        do {
            //try sendSocket.bind(toPort: 4211)
            try sendSocket.enableBroadcast(true)
            //try socket.connect()
        } catch {
            print(error)
        }
        
    }
    
    @IBAction func sliderValueChanged(_ sender: Any) {
        let slider = sender as! UISlider
        
        targetTemp = Int16(slider.value)
        //sendSocket.send(message!, toHost: "104.38.61.33", port: 4211, withTimeout: 1000, tag: 0)
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
        
        currentTemp = statuses.currentTemp
        
        if (statuses.mainValveClosed == true) {
            mainValveLabel.text = "Main Valve: Closed"
        } else {
            mainValveLabel.text = "Main Valve: Open"
        }
        
        coldValveLabel.text = "Cold Valve: \(statuses.servoAngle)º"
        
        if (statuses.showerReady == true) {
            showerReadyLabel.text = "Shower Ready: Yes"
            alert2Stepper.isEnabled = false
            alert1Stepper.isEnabled = false
            sliderControl.isEnabled = false
            //confirmButton.isEnabled = false
            
            //Force update to default vals
            expectingUpdate = true
            
        } else {
            showerReadyLabel.text = "Shower Ready: No"
        }
        
        if (expectingUpdate) {
            alert1Time = (statuses.timer1 / 60000)
            alert2Time = (statuses.timer2 / 60000)
            targetTemp = statuses.targetTemp
            sliderControl.value = Float(statuses.targetTemp)
            //alert1Stepper.value = Double(statuses.timer1)
            //alert2Stepper.value = Double(statuses.timer2)
            expectingUpdate = false
        }
        
        alert.dismiss(animated: true, completion: nil)
    }
    
    func sendUpdate() {
        //let encode = Enco
        
        //let encoder = JSONEncoder()
        
        //let data2 = NSData(bytesNoCopy: &responses, length: sizeof(responseStats), freeWhenDone: false)
        
        //let data = responseStats.archive(w: responses)
        
        //let data = Data(bytes: &responses, count: MemoryLayout.size(ofValue: responseStats.self))
        var data = Data()
        //data.append
        
        var time1 = responses.timer1 * 60000
        var time2 = responses.timer2 * 60000

        data.append(Data(bytes: &time1, count: 4))
        data.append(Data(bytes: &time2, count: 4))
        data.append(Data(bytes: &responses.targetTemp, count: 2))
        //data.append(Data(bytes: &responses.targetTemp, count: 2))
        //data.append(responses.timer2)
        //data.append(responses.targetTemp)
        //data.append(responses.targetTemp)
        //print(data.byt)
        
        sendSocket.send(data, toHost: "255.255.255.255", port: 4211, withTimeout: 1000, tag: 0)
        
        //expectingUpdate = true
    }
    
    @IBAction func confirmButtonPressed(_ sender: UIButton) {
        sendUpdate()
    }
    
    @IBAction func stepper1Changed(_ sender: UIStepper) {
        alert1Time = Int32(sender.value)
    }
    
    @IBAction func stepper2Changed(_ sender: UIStepper) {
        alert2Time = Int32(sender.value)
    }
}

