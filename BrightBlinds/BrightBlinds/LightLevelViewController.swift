//
//  LightLevelViewController.swift
//  BrightBlinds
//
//  Created by Vinh Vu on 3/14/20.
//  Copyright © 2020 Vinh Vu. All rights reserved.
//

import UIKit
import CoreBluetooth

class LightLevelViewController: UIViewController {

    var myPeripheral: CBPeripheral!
    var butChar: CBCharacteristic?
    
    override func viewDidLoad() {
        super.viewDidLoad()
    }
    
    @IBAction func twoBut(_ sender: Any) {
        print("Two Button Sending")
        let twoString: String = "2"
        let twoData = Data(twoString.utf8)
        myPeripheral.writeValue(twoData, for: butChar!, type: CBCharacteristicWriteType.withoutResponse)
    }
    
    @IBAction func fourBut(_ sender: Any) {
        print("Four Button Sending")
        let fourString: String = "4"
        let fourData = Data(fourString.utf8)
        myPeripheral.writeValue(fourData, for: butChar!, type: CBCharacteristicWriteType.withoutResponse)
    }
    
    @IBAction func sixBut(_ sender: Any) {
        print("Six Button Sending")
        let sixString: String = "6"
        let sixData = Data(sixString.utf8)
        myPeripheral.writeValue(sixData, for: butChar!, type: CBCharacteristicWriteType.withoutResponse)
    }
    
    @IBAction func eightBut(_ sender: Any) {
        print("Eight Button Sending")
        let eightString: String = "-"
        let eightData = Data(eightString.utf8)
        myPeripheral.writeValue(eightData, for: butChar!, type: CBCharacteristicWriteType.withoutResponse)
    }

    @IBAction func backBut(_ sender: Any) {
        print("Back Button Sending")
        let backString: String = "q"
        let backData = Data(backString.utf8)
        myPeripheral.writeValue(backData, for: butChar!, type: CBCharacteristicWriteType.withoutResponse)
    }
    
    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        let vc = segue.destination as! ViewController
        vc.butChar = self.butChar
        let per = segue.destination as! ViewController
        per.myPeripheral = self.myPeripheral
    }
    
    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
    }

}
