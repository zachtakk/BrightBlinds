//
//  ManualViewController.swift
//  BrightBlinds
//
//  Created by Vinh Vu on 3/14/20.
//  Copyright © 2020 Vinh Vu. All rights reserved.
//

import UIKit
import CoreBluetooth

class ManualViewController: UIViewController{
    
    var myPeripheral: CBPeripheral!
    var butChar: CBCharacteristic?

    override func viewDidLoad() {
        super.viewDidLoad()
    }

    @IBAction func downBut(_ sender: Any) {
        print("Closing Button Sending")
        let downString: String = "-"
        let downData = Data(downString.utf8)
        myPeripheral.writeValue(downData, for: butChar!, type: CBCharacteristicWriteType.withoutResponse)
    }
    
    @IBAction func upBut(_ sender: Any) {
        print("Opening Button Sending")
        let upString: String = "+"
        let upData = Data(upString.utf8)
        myPeripheral.writeValue(upData, for: butChar!, type: CBCharacteristicWriteType.withoutResponse)
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
