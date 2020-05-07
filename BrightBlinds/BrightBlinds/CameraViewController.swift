//
//  CameraViewController.swift
//  BrightBlinds
//
//  Created by Vinh Vu on 3/14/20.
//  Copyright © 2020 Vinh Vu. All rights reserved.
//

import UIKit
import CoreBluetooth

class CameraViewController: UIViewController {
    var myPeripheral: CBPeripheral!
    var butChar: CBCharacteristic?
    
    override func viewDidLoad() {
        super.viewDidLoad()

        // Do any additional setup after loading the view.
    }
    

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
    }
    

}
