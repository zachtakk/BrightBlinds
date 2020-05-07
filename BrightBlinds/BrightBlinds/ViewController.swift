//
//  ViewController.swift
//  BrightBlinds
//
//  Created by Vinh Vu on 1/11/20.
//  Copyright © 2020 Vinh Vu. All rights reserved.
//

import UIKit
import CoreBluetooth

let svcBLE = CBUUID.init(string: "FFE0")

class ViewController: UIViewController, CBCentralManagerDelegate, CBPeripheralDelegate {
    
    // Properties
    var centralManager: CBCentralManager!
    var myPeripheral: CBPeripheral!
    
    // Characteristic var
    var _characteristics: CBCharacteristic?
    var butChar: CBCharacteristic?
    
    override func viewDidLoad() {
        super.viewDidLoad()
        centralManager = CBCentralManager.init(delegate: self, queue: nil)
    }
    
    func centralManagerDidUpdateState(_ central: CBCentralManager) {
        if central.state == CBManagerState.poweredOn{
            central.scanForPeripherals(withServices: nil, options: nil)
        }
    }
    
    func centralManager(_ central: CBCentralManager, didDiscover peripheral: CBPeripheral, advertisementData: [String : Any], rssi RSSI: NSNumber) {
        if peripheral.name?.contains("Bright") == true {
            print(peripheral.name ?? "no name")
            centralManager.stopScan()
            self.myPeripheral = peripheral
            self.centralManager.connect(peripheral, options: nil)
        }
    }
    
    func centralManager(_ central: CBCentralManager, didConnect peripheral: CBPeripheral) {
        print("connected \(String(describing: peripheral.name))")
        peripheral.discoverServices(nil)
        peripheral.delegate = self
    }
    
    func peripheral(_ peripheral: CBPeripheral, didDiscoverServices error: Error?) {
        if let services = peripheral.services{
            for svc in services{
                if svc.uuid == svcBLE{
                    peripheral.discoverCharacteristics(nil, for: svc)
                }
            }
        }
    }
    
    func peripheral(_ peripheral: CBPeripheral, didDiscoverCharacteristicsFor service: CBService, error: Error?) {
        let _characteristics = service.characteristics
        for characteristic in _characteristics!{
            if(characteristic.uuid.uuidString == "FFE1"){
                butChar = characteristic
            }
        }
    }
    
    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
    }
    
    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        if segue.identifier == "m"{
            let vc1 = segue.destination as! ManualViewController
            vc1.butChar = self.butChar
            let per1 = segue.destination as! ManualViewController
            per1.myPeripheral = self.myPeripheral
        }
        else if segue.identifier  == "l"{
            let vc2 = segue.destination as! LightLevelViewController
            vc2.butChar = self.butChar
            let per2 = segue.destination as! LightLevelViewController
            per2.myPeripheral = self.myPeripheral
        }
        else if segue.identifier == "s"{
            print("Schedule Move")
            let vc3 = segue.destination as! SchedulingStartViewController
            vc3.butChar = self.butChar
            let per3 = segue.destination as! SchedulingStartViewController
            per3.myPeripheral = self.myPeripheral
        }
        else if segue.identifier == "C"{
            print("Cam Move")
            let vc4 = segue.destination as! CameraViewController
            vc4.butChar = self.butChar
            let per4 = segue.destination as! CameraViewController
            per4.myPeripheral = self.myPeripheral
        }
    }
    
    @IBAction func manualBut(_ sender: UIButton) {
        print("Manual Button Sending")
        let string1: String = "m"
        let data1 = Data(string1.utf8)
        myPeripheral!.writeValue(data1, for: butChar!, type: CBCharacteristicWriteType.withoutResponse)
    }
    
    @IBAction func lightBut(_ sender: UIButton) {
        print("Light Button Sending")
        let string2: String = "l"
        let data2 = Data(string2.utf8)
        myPeripheral!.writeValue(data2, for: butChar!, type: CBCharacteristicWriteType.withoutResponse)
        
    }
    
    @IBAction func scheBut(_ sender: UIButton) {
        print("Schedule Button Sending")
        let string3: String = "s"
        let data3 = Data(string3.utf8)
        myPeripheral!.writeValue(data3, for: butChar!, type: CBCharacteristicWriteType.withoutResponse)
    }
    
    
    @IBAction func camBut(_ sender: UIButton) {
        print("Camera Button Sending")
        let string4: String = "C"
        let data4 = Data(string4.utf8)
        myPeripheral!.writeValue(data4, for: butChar!, type: CBCharacteristicWriteType.withoutResponse)
    }
    
    
}

