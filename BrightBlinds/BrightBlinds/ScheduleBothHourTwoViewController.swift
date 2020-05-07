//
//  ScheduleBothHourTwoViewController.swift
//  BrightBlinds
//
//  Created by Vinh Vu on 4/12/20.
//  Copyright © 2020 Vinh Vu. All rights reserved.
//

import UIKit
import CoreBluetooth

class ScheduleBothHourTwoViewController: UIViewController, UIPickerViewDataSource, UIPickerViewDelegate {
    let dataSource = ["6 AM","7 AM", "8 AM", "9 AM", "10 AM", "11 AM", "12 PM", "1 PM", "2 PM", "3 PM", "4 PM", "5 PM", "6 PM", "7PM", "8PM", "9PM", "10PM", "11PM"]
    private let bleSource = ["06", "07", "08", "09", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23"]
    
    @IBOutlet weak var label3: UILabel!
    @IBOutlet weak var picker3: UIPickerView!
    
    var myPeripheral: CBPeripheral!
    var butChar: CBCharacteristic?
    var index: Int = 0
    
    override func viewDidLoad() {
        super.viewDidLoad()
        picker3.delegate = self
        picker3.dataSource = self
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
        label3.text = dataSource[row]
        index = row
    }
    
    
    @IBAction func h2But(_ sender: UIButton) {
        print("Hour Two Button Sending")
        let h2String: String = bleSource[index]
        print(h2String)
        let h2Data = Data(h2String.utf8)
        myPeripheral.writeValue(h2Data, for: butChar!, type: CBCharacteristicWriteType.withoutResponse)
    }
    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        if segue.identifier == "H2"{
            let vc = segue.destination as! ScheduleBothMinuteTwoViewController
            vc.butChar = self.butChar
            let per = segue.destination as! ScheduleBothMinuteTwoViewController
            per.myPeripheral = self.myPeripheral
        }
    }
}
