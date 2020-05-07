//
//  ScheduleBothMinuteOneViewController.swift
//  BrightBlinds
//
//  Created by Vinh Vu on 4/12/20.
//  Copyright © 2020 Vinh Vu. All rights reserved.
//

import UIKit
import CoreBluetooth

class ScheduleBothMinuteOneViewController: UIViewController, UIPickerViewDataSource, UIPickerViewDelegate {
    let dataSource = ["00","01","02","03","04","05","06","07","08","09","10","11","12","13","14","15","16","17","18","19", "20","21","22","23","24","25","26","27","28","29","30", "31","32","33","34","35","36","37","38","39","40","41","42","43","44","45","46","47","48","49","50","51","52","53","54","55","56","57","58","59"]
    
    @IBOutlet weak var label2: UILabel!
    @IBOutlet weak var picker2: UIPickerView!
    
    var myPeripheral: CBPeripheral!
    var butChar: CBCharacteristic?
    var index: Int = 0
    
    override func viewDidLoad() {
        super.viewDidLoad()
        picker2.delegate = self
        picker2.dataSource = self
        // Do any additional setup after loading the view.
    }
    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
    }
    func numberOfComponents(in pickerView: UIPickerView) -> Int {
        return 1
    }
    func pickerView(_ pickerView: UIPickerView, titleForRow row: Int, forComponent component: Int) -> String? {
        return dataSource[row]
    }
    func pickerView(_ pickerView: UIPickerView, numberOfRowsInComponent component: Int) -> Int {
        return dataSource.count
    }
    func pickerView(_ pickerView: UIPickerView, didSelectRow row: Int, inComponent component: Int) {
        label2.text = dataSource[row]
        index = row
    }

    @IBAction func m1But(_ sender: UIButton) {
        print("Minute One Button Sending")
        let m1String: String = dataSource[index]
        print(m1String)
        let m1Data = Data(m1String.utf8)
        myPeripheral.writeValue(m1Data, for: butChar!, type: CBCharacteristicWriteType.withoutResponse)
    }
    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        if segue.identifier == "M1"{
            let vc = segue.destination as! ScheduleBothHourTwoViewController
            vc.butChar = self.butChar
            let per = segue.destination as! ScheduleBothHourTwoViewController
            per.myPeripheral = self.myPeripheral
        }
    }
}
