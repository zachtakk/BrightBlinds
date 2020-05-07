//
//  SchedulingStartViewController.swift
//  BrightBlinds
//
//  Created by Vinh Vu on 4/16/20.
//  Copyright © 2020 Vinh Vu. All rights reserved.
//

import UIKit
import CoreBluetooth

class SchedulingStartViewController: UIViewController {

    var myPeripheral: CBPeripheral!
    var butChar: CBCharacteristic?
    
    override func viewDidLoad() {
        super.viewDidLoad()
    }
    
    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        if segue.identifier == "q"{
            let vc = segue.destination as! ViewController
            vc.butChar = self.butChar
            let per = segue.destination as! ViewController
            per.myPeripheral = self.myPeripheral
        }
        else if segue.identifier == "o"{
            let vc = segue.destination as! SchedulingViewController
            vc.butChar = self.butChar
            let per = segue.destination as! SchedulingViewController
            per.myPeripheral = self.myPeripheral
        }
        else if segue.identifier == "c"{
            let vc = segue.destination as! SchedulingViewController
            vc.butChar = self.butChar
            let per = segue.destination as! SchedulingViewController
            per.myPeripheral = self.myPeripheral
        }
        else if segue.identifier == "b"{
            let vc = segue.destination as! ScheduleBothHourOneViewController
            vc.butChar = self.butChar
            let per = segue.destination as! ScheduleBothHourOneViewController
            per.myPeripheral = self.myPeripheral
        }
    }
    
    @IBAction func openBut(_ sender: UIButton) {
        print("Open Button Sending")
        let openString: String = "o"
        let openData = Data(openString.utf8)
        myPeripheral.writeValue(openData, for: butChar!, type: CBCharacteristicWriteType.withoutResponse)
    }
    
    @IBAction func closeBut(_ sender: UIButton) {
        print("Close Button Sending")
        let closeString: String = "c"
        let closeData = Data(closeString.utf8)
        myPeripheral.writeValue(closeData, for: butChar!, type: CBCharacteristicWriteType.withoutResponse)
    }
    
    @IBAction func bothBut(_ sender: UIButton) {
        print("Both Button Sending")
        let bothString: String = "b"
        let bothData = Data(bothString.utf8)
        myPeripheral.writeValue(bothData, for: butChar!, type: CBCharacteristicWriteType.withoutResponse)
    }
    
    @IBAction func backBut(_ sender: UIButton) {
        print("Back Button Sending")
        let backString: String = "q"
        let backData = Data(backString.utf8)
        myPeripheral.writeValue(backData, for: butChar!, type: CBCharacteristicWriteType.withoutResponse)
    }
    
    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
    }
}


