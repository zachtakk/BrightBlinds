//
//  ScheduleBothHourOneViewController.swift
//  BrightBlinds
//
//  Created by Vinh Vu on 4/12/20.
//  Copyright © 2020 Vinh Vu. All rights reserved.
//

import UIKit
import CoreBluetooth

class ScheduleBothHourOneViewController: UIViewController, UIPickerViewDataSource, UIPickerViewDelegate {
    let dataSource = ["6 AM","7 AM", "8 AM", "9 AM", "10 AM", "11 AM", "12 PM", "1 PM", "2 PM", "3 PM", "4 PM", "5 PM", "6 PM", "7PM", "8PM", "9PM", "10PM", "11PM"]
    private let bleSource = ["06", "07", "08", "09", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23"]
    
    var myPeripheral: CBPeripheral!
    var butChar: CBCharacteristic?
    var index: Int = 0
    
    @IBOutlet weak var label1: UILabel!
    @IBOutlet weak var picker1: UIPickerView!
    override func viewDidLoad() {
        picker1.delegate = self
        picker1.dataSource = self
        super.viewDidLoad()
        // Do any additional setup after loading the view.
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
        label1.text = dataSource[row]
        index = row
    }
    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
    }

    @IBAction func h1But(_ sender: UIButton) {
        print("Hour One Button Sending")
        let h1String: String = bleSource[index]
        print(h1String)
        let h1Data = Data(h1String.utf8)
        myPeripheral.writeValue(h1Data, for: butChar!, type: CBCharacteristicWriteType.withoutResponse)
    }
    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        if segue.identifier == "H1"{
            let vc = segue.destination as! ScheduleBothMinuteOneViewController
            vc.butChar = self.butChar
            let per = segue.destination as! ScheduleBothMinuteOneViewController
            per.myPeripheral = self.myPeripheral
        }
    }
    
}
